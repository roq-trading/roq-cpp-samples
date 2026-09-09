/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/flags/args.hpp"

#include "roq/logging.hpp"

#include "roq/logging/flags/settings.hpp"

#include "roq/service.hpp"

#include "roq/utils/common.hpp"
#include "roq/utils/update.hpp"

#include "roq/market/utils.hpp"

#include "roq/client.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
auto const INFO = roq::Service::Info{
    .description = "Maker (Roq Samples)"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};

auto const ORDER_CANCEL_POLICY = roq::OrderCancelPolicy::BY_ACCOUNT;

auto const EXECUTION_INSTRUCTIONS = roq::Mask{
    roq::ExecutionInstruction::PARTICIPATE_DO_NOT_INITIATE,
};

auto const ACCOUNT = "A1"sv;

auto const EXCHANGE = "deribit"sv;
auto const SYMBOL = "BTC-PERPETUAL"sv;

auto const SPREAD = 1.0;

auto const QUANTITY = 1.0;
}  // namespace

// === HELPERS ===

namespace {

struct Config final : public roq::client::Config {
 protected:
  void dispatch(Handler &handler) const override {
    handler(
        roq::client::Settings{
            .order_cancel_policy = ORDER_CANCEL_POLICY,
            .order_management = {},
        });
    handler(
        roq::client::Account{
            .regex = ACCOUNT,
        });
    handler(
        roq::client::Symbol{
            .regex = SYMBOL,
            .exchange = EXCHANGE,
        });
  }
};

struct Shared final {
  explicit Shared(roq::client::Dispatcher &dispatcher) : dispatcher{dispatcher} {}

  roq::client::Dispatcher &dispatcher;
  uint64_t max_order_id = {};
};

struct Quote final {
  Quote(roq::Side side, Shared &shared) : side_{side}, shared_{shared} {}

  void clear() {
    target_price_ = roq::NaN;
    update_order_ = {};
    order_id_ = {};
    limit_price_ = roq::NaN;
  }

  bool update_target_price(double price) {
    if (roq::utils::update(target_price_, price)) {
      update_order_ = true;
      return true;
    }
    return false;
  }

  void operator()(roq::Event<roq::Timer> const &) {
    if (!update_order_) {
      return;
    }
    switch (state_) {
      using enum State;
      case UNDEFINED:
        create_order();
        break;
      case CREATE:
      case MODIFY:
      case CANCEL:
        break;
      case WORKING:
        cancel_order();
        break;
    }
  }

  void operator()(roq::Event<roq::OrderAck> const &event) {
    auto &[message_info, order_ack] = event;
    if (roq::utils::has_request_failed(order_ack.request_status)) {
      // could be valid rejects like too-late to cancel
      roq::log::fatal("Unexpected"sv);
    }
    if (roq::utils::has_request_completed(order_ack.request_status)) {
      switch (order_ack.request_type) {
        using enum roq::RequestType;
        case UNDEFINED:
          roq::log::fatal("Unexpected"sv);
        case CREATE_ORDER:
          if (state_ != State::CREATE) {
            roq::log::fatal("Unexpected: state={}"sv, state_);
          }
          (*this)(State::WORKING);
          break;
        case MODIFY_ORDER:
          if (state_ != State::MODIFY) {
            roq::log::fatal("Unexpected: state={}"sv, state_);
          }
          (*this)(State::WORKING);
          break;
        case CANCEL_ORDER:
          if (state_ != State::CANCEL) {
            roq::log::fatal("Unexpected: state={}"sv, state_);
          }
          (*this)(State::WORKING);
          break;
      }
    }
    // - rate limit
  }

 protected:
  enum class State {
    UNDEFINED,
    CREATE,
    MODIFY,
    CANCEL,
    WORKING,
  };

  void operator()(State state) {
    if (roq::utils::update(state_, state)) {
      roq::log::warn("state={}"sv, state_);
    }
  }

  void create_order() {
    // TODO try-catch
    if (state_ != State::UNDEFINED) [[unlikely]] {
      roq::log::fatal("Unexpected: state={}"sv, state_);
    }
    if (std::isnan(target_price_)) {
      roq::log::fatal("Unexpected: target_price={}"sv, target_price_);
    }
    auto order_id = ++shared_.max_order_id;
    limit_price_ = target_price_;
    roq::log::warn("side={}, limit_price={} ==> {}"sv, side_, limit_price_, order_id);
    auto create_order = roq::CreateOrder{
        .account = ACCOUNT,
        .order_id = order_id,
        .exchange = EXCHANGE,
        .symbol = SYMBOL,
        .side = side_,
        .position_effect = {},
        .margin_mode = {},
        .quantity_type = {},
        .max_show_quantity = roq::NaN,
        .order_type = roq::OrderType::LIMIT,
        .time_in_force = roq::TimeInForce::GTC,
        .execution_instructions = EXECUTION_INSTRUCTIONS,
        .request_template = {},
        .quantity = QUANTITY,
        .price = limit_price_,
        .stop_price = roq::NaN,
        .leverage = roq::NaN,
        .routing_id = {},
        .strategy_id = {},
        .release_time_utc = {},
    };
    shared_.dispatcher.send(create_order, 0);
    order_id_ = order_id;
    (*this)(State::CREATE);
  }

  void cancel_order() {
    // TODO try-catch
    if (state_ != State::WORKING) [[unlikely]] {
      roq::log::fatal("Unexpected: state={}"sv, state_);
    }
    auto cancel_order = roq::CancelOrder{
        .account = ACCOUNT,
        .order_id = order_id_,
        .request_template = {},
        .routing_id = {},
        .version = {},
        .conditional_on_version = {},
        .release_time_utc = {},
    };
    shared_.dispatcher.send(cancel_order, 0);
    (*this)(State::CANCEL);
  }

 private:
  roq::Side const side_;
  Shared &shared_;
  double target_price_ = roq::NaN;
  bool update_order_ = {};
  uint64_t order_id_ = {};
  double limit_price_ = roq::NaN;
  State state_ = {};
};

struct Strategy final : public roq::client::Handler {
  explicit Strategy(roq::client::Dispatcher &dispatcher) : shared_{dispatcher}, bid_{roq::Side::BUY, shared_}, ask_{roq::Side::SELL, shared_} {}

 protected:
  enum class State {
    UNDEFINED,
    CANCEL_ALL,
    QUOTE,
  };

  void operator()(roq::Event<roq::Timer> const &event) override {
    if (can_trade()) {
      bid_(event);
      ask_(event);
    }
  }

  void operator()(roq::Event<roq::Disconnected> const &) override {
    ready_ = false;
    state_ = {};
    mid_price_ = roq::NaN;
    bid_.clear();
    ask_.clear();
  }

  void operator()(roq::Event<roq::DownloadEnd> const &event) override {
    auto &[message_info, download_end] = event;
    if (roq::utils::update_max(shared_.max_order_id, download_end.max_order_id)) {
      roq::log::warn("max_order_id={}"sv, shared_.max_order_id);
    }
  }

  void operator()(roq::Event<roq::Ready> const &) override {
    ready_ = true;
    (*this)(State::CANCEL_ALL);
    cancel_all_orders();
  }

  void operator()(roq::Event<roq::ReferenceData> const &event) override {
    auto &[message_info, reference_data] = event;
    if (roq::utils::update(tick_size_, reference_data.tick_size)) {
      roq::log::warn("tick_size={}"sv, tick_size_);
    }
  }

  void operator()(roq::Event<roq::MarketStatus> const &event) override {
    auto &[message_info, market_status] = event;
    if (roq::utils::update(trading_status_, market_status.trading_status)) {
      roq::log::warn("trading_status={}"sv, trading_status_);
    }
  }

  void operator()(roq::Event<roq::TopOfBook> const &event) override {
    auto &[message_info, top_of_book] = event;
    if (ready_) {
      auto mid_price = 0.5 * (top_of_book.layer.bid_price + top_of_book.layer.ask_price);
      if (roq::utils::update(mid_price_, mid_price)) {
        update_quotes();
      }
    }
    // we may want to move either side fast if it moves against us
  }

  // order management

  void operator()(roq::Event<roq::CancelAllOrdersAck> const &) override {
    // note! don't care about the actual result (best effort), just advance state
    if (ready_ && state_ == State::CANCEL_ALL) {
      (*this)(State::QUOTE);
    }
  }

  void operator()(roq::Event<roq::OrderAck> const &event) override {
    auto &[message_info, order_ack] = event;
    switch (order_ack.side) {
      using enum roq::Side;
      case UNDEFINED:
        roq::log::fatal("Unexpected"sv);
      case BUY:
        bid_(event);
        break;
      case SELL:
        ask_(event);
        break;
    }
  }

  void operator()(roq::Event<roq::OrderUpdate> const &) override {
    // update virtual position
  }

  void operator()(roq::Event<roq::TradeUpdate> const &) override {}

  // real-time position feed

  void operator()(roq::Event<roq::PortfolioUpdate> const &) override {}

  // helpers

  void operator()(State state) {
    if (roq::utils::update(state_, state)) {
      roq::log::warn("state={}"sv, state_);
    }
  }

  bool can_trade() const { return ready_ && state_ == State::QUOTE && !std::isnan(tick_size_) && trading_status_ == roq::TradingStatus::OPEN; }

  void cancel_all_orders() const {
    roq::log::warn("*** CANCEL ALL ORDERS ***"sv);
    auto cancel_all_orders = roq::CancelAllOrders{
        .account = ACCOUNT,
        .order_id = {},
        .exchange = EXCHANGE,
        .symbol = SYMBOL,
        .strategy_id = {},
        .side = {},
    };
    shared_.dispatcher.send(cancel_all_orders, 0);
  }

  void update_quotes() {
    if (std::isnan(mid_price_)) [[unlikely]] {
    } else {
      auto half_spread = 0.5 * SPREAD * tick_size_;
      auto bid_price = roq::market::round_away(roq::Side::BUY, mid_price_ - half_spread, tick_size_);
      auto ask_price = roq::market::round_away(roq::Side::SELL, mid_price_ + half_spread, tick_size_);
      bid_.update_target_price(bid_price);
      ask_.update_target_price(ask_price);
    }
  }

 private:
  Shared shared_;
  bool ready_ = false;
  State state_ = {};
  uint64_t max_order_id_ = {};
  double tick_size_ = roq::NaN;
  roq::TradingStatus trading_status_ = {};
  double mid_price_ = roq::NaN;
  Quote bid_;
  Quote ask_;
};

struct Application final : public roq::Service {
  using roq::Service::Service;

 protected:
  int main(roq::args::Parser const &args) override {
    auto params = args.params();
    if (std::empty(params)) {
      roq::log::fatal("Expected arguments"sv);
    }
    roq::client::flags::Settings settings{args};
    Config config;
    roq::client::Trader{settings, config, params}.dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  roq::flags::Args args{argc, argv, INFO.description, INFO.build_version};
  roq::logging::flags::Settings settings{args};
  return Application{args, settings, INFO}.run();
}

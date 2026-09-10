/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include <deque>

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

// TODO
// - order_update completed/canceled => check order_id when order_ack/order_update is delayed
// - direction of tick => process bid or ask first
// - rate limiter

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

auto const SPREAD = 1.5;

auto const QUANTITY = 1.0;

size_t const REQUEST_LIMIT = 10;
auto const MONITOR_PERIOD = 2s;
}  // namespace

// === HELPERS ===

namespace roq {
namespace maker {

struct Config final : public client::Config {
 protected:
  void dispatch(Handler &handler) const override {
    handler(
        client::Settings{
            .order_cancel_policy = ORDER_CANCEL_POLICY,
            .order_management = {},
        });
    handler(
        client::Account{
            .regex = ACCOUNT,
        });
    handler(
        client::Symbol{
            .regex = SYMBOL,
            .exchange = EXCHANGE,
        });
  }
};

struct Shared final {
  explicit Shared(client::Dispatcher &dispatcher) : dispatcher{dispatcher} {}

  client::Dispatcher &dispatcher;
  uint64_t max_order_id = {};

  double position = 0.0;

  std::deque<std::chrono::nanoseconds> history;

  bool is_blocked(std::chrono::nanoseconds now) const { return std::size(history) == REQUEST_LIMIT && (now - history.front()) < MONITOR_PERIOD; }

  template <typename Callback>
  bool rate_limit(std::chrono::nanoseconds now, Callback callback) {
    if (is_blocked(now)) {
      return false;
    }
    if (!std::empty(history)) {
      log::warn(
          "RATE LIMIT size={}, front={}, back={}, age={}"sv,
          std::size(history),
          history.front(),
          history.back(),
          std::chrono::duration_cast<std::chrono::seconds>(now - history.front()));
    }
    callback();
    auto cutoff = now - MONITOR_PERIOD;
    while (!std::empty(history) && history.front() < cutoff) {
      history.pop_front();
    }
    history.push_back(now);
    if (is_blocked(now)) {
      log::warn("*** BLOCK ***"sv);
    }
    return true;
  }
};

struct Quote final {
  Quote(Side side, Shared &shared) : side_{side}, shared_{shared} {}

  void clear() {
    target_price_ = NaN;
    update_order_ = {};
    order_id_ = {};
    limit_price_ = NaN;
  }

  bool update_target_price(double price) {
    if (utils::update(target_price_, price)) {
      log::warn("[{}] TARGET {}"sv, side_, target_price_);
      update_order_ = true;
      return true;
    }
    return false;
  }

  void operator()(Event<Timer> const &event) {
    if (!update_order_) {
      return;
    }
    switch (state_) {
      using enum State;
      case UNDEFINED:
        create_order(event);
        break;
      case CREATE:
      case MODIFY:
      case CANCEL:
        break;
      case WORKING:
        cancel_order(event);
        break;
    }
    update_order_ = false;
  }

  void operator()(Event<OrderAck> const &event) {
    auto &[message_info, order_ack] = event;
    auto failure = utils::has_request_failed(order_ack.request_status);
    auto success = utils::has_request_succeeded(order_ack.request_status);
    if (failure || success) {
      log::info("[{}] order_ack={}"sv, side_, order_ack);
    }
    if (failure) {
      switch (order_ack.error) {
        using enum Error;
        case TOO_LATE_TO_MODIFY_OR_CANCEL:
          break;
        default:
          log::fatal("[{}] Unexpected"sv, side_);
      }
    }
    if (utils::has_request_completed(order_ack.request_status)) {
      switch (order_ack.request_type) {
        using enum RequestType;
        case UNDEFINED:
          log::fatal("[{}] Unexpected"sv, side_);
        case CREATE_ORDER:
          if (state_ != State::CREATE) {
            log::fatal("[{}] Unexpected: state={}"sv, side_, state_);
          }
          (*this)(State::WORKING);
          break;
        case MODIFY_ORDER:
          if (state_ != State::MODIFY) {
            log::fatal("[{}] Unexpected: state={}"sv, side_, state_);
          }
          (*this)(State::WORKING);
          break;
        case CANCEL_ORDER:
          if (state_ != State::CANCEL) {
            log::fatal("[{}] Unexpected: state={}"sv, side_, state_);
          }
          order_id_ = {};
          (*this)(State::UNDEFINED);
          break;
      }
    }
    // - rate limit
  }

  // XXX FIXME TODO drop download
  void operator()(Event<OrderUpdate> const &event) {
    auto &[message_info, order_update] = event;
    if (order_update.update_type != UpdateType::INCREMENTAL) {
      return;
    }
    log::info("[{}] order_update={}"sv, side_, order_update);
    if (order_update.order_status == OrderStatus::COMPLETED) {
      log::warn("[{}] FILL {} @ {}"sv, side_, order_update.quantity, order_update.price);
      shared_.position += utils::sign(side_) * order_update.quantity;
      log::warn("[{}] POSITION {}"sv, side_, shared_.position);
    }
    if (utils::is_order_complete(order_update.order_status)) {
      if (order_id_ == order_update.order_id) {
        order_id_ = {};
        (*this)(State::UNDEFINED);
      }
    }
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
    if (utils::update(state_, state)) {
      log::info("[{}] state={}"sv, side_, state_);
    }
  }

  void create_order(MessageInfo const &message_info) {
    auto order_id = shared_.max_order_id + 1;
    log::warn("[{}] CREATE {} {}"sv, side_, order_id, target_price_);
    if (state_ != State::UNDEFINED) [[unlikely]] {
      log::fatal("[{}] Unexpected: state={}"sv, side_, state_);
    }
    if (std::isnan(target_price_)) {
      log::fatal("[{}] Unexpected: target_price={}"sv, side_, target_price_);
    }
    auto create_order = CreateOrder{
        .account = ACCOUNT,
        .order_id = order_id,
        .exchange = EXCHANGE,
        .symbol = SYMBOL,
        .side = side_,
        .position_effect = {},
        .margin_mode = {},
        .quantity_type = {},
        .max_show_quantity = NaN,
        .order_type = OrderType::LIMIT,
        .time_in_force = TimeInForce::GTC,
        .execution_instructions = EXECUTION_INSTRUCTIONS,
        .request_template = {},
        .quantity = QUANTITY,
        .price = target_price_,
        .stop_price = NaN,
        .leverage = NaN,
        .routing_id = {},
        .strategy_id = {},
        .release_time_utc = {},
    };
    log::info("[{}] create_order={}"sv, side_, create_order);
    auto helper = [&]() {
      shared_.dispatcher.send(create_order, 0);
      assert(shared_.max_order_id < order_id);
      shared_.max_order_id = order_id;
      assert(order_id_ == 0);
      order_id_ = order_id;
      (*this)(State::CREATE);
    };
    try {
      shared_.rate_limit(message_info.origin_create_time, helper);
    } catch (NotConnected const &e) {
      log::error("{}"sv, e);
    }
  }

  void cancel_order(MessageInfo const &message_info) {
    log::warn("[{}] CANCEL {}"sv, side_, order_id_);
    if (state_ != State::WORKING) [[unlikely]] {
      log::fatal("[{}] Unexpected: state={}"sv, side_, state_);
    }
    if (order_id_ == 0) {
      return;
    }
    auto cancel_order = CancelOrder{
        .account = ACCOUNT,
        .order_id = order_id_,
        .request_template = {},
        .routing_id = {},
        .version = {},
        .conditional_on_version = {},
        .release_time_utc = {},
    };
    log::info("[{}] cancel_order={}"sv, side_, cancel_order);
    auto helper = [&]() {
      shared_.dispatcher.send(cancel_order, 0);
      (*this)(State::CANCEL);
    };
    try {
      shared_.rate_limit(message_info.origin_create_time, helper);
    } catch (NotConnected const &e) {
      log::error("{}"sv, e);
    }
  }

 private:
  Side const side_;
  Shared &shared_;
  double target_price_ = NaN;
  bool update_order_ = {};
  uint64_t order_id_ = {};
  double limit_price_ = NaN;
  State state_ = {};
};

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &dispatcher) : shared_{dispatcher}, bid_{Side::BUY, shared_}, ask_{Side::SELL, shared_} {}

 protected:
  enum class State {
    UNDEFINED,
    CANCEL_ALL,
    QUOTE,
  };

  void operator()(Event<Timer> const &event) override {
    if (can_trade()) {
      bid_(event);
      ask_(event);
    }
  }

  void operator()(Event<Disconnected> const &) override {
    ready_ = false;
    state_ = {};
    mid_price_ = NaN;
    bid_.clear();
    ask_.clear();
  }

  void operator()(Event<DownloadEnd> const &event) override {
    auto &[message_info, download_end] = event;
    if (utils::update_max(shared_.max_order_id, download_end.max_order_id)) {
      log::info("max_order_id={}"sv, shared_.max_order_id);
    }
  }

  void operator()(Event<Ready> const &) override {
    ready_ = true;
    (*this)(State::CANCEL_ALL);
    cancel_all_orders();
  }

  void operator()(Event<ReferenceData> const &event) override {
    auto &[message_info, reference_data] = event;
    if (utils::update(tick_size_, reference_data.tick_size)) {
      log::info("tick_size={}"sv, tick_size_);
    }
  }

  void operator()(Event<MarketStatus> const &event) override {
    auto &[message_info, market_status] = event;
    if (utils::update(trading_status_, market_status.trading_status)) {
      log::info("trading_status={}"sv, trading_status_);
    }
  }

  void operator()(Event<TopOfBook> const &event) override {
    auto &[message_info, top_of_book] = event;
    if (ready_) {
      auto mid_price = 0.5 * (top_of_book.layer.bid_price + top_of_book.layer.ask_price);
      if (utils::update(mid_price_, mid_price)) {
        update_quotes();
      }
    }
    // we may want to move either side fast if it moves against us
  }

  // order management

  void operator()(Event<CancelAllOrdersAck> const &) override {
    // note! don't care about the actual result (best effort), just advance state
    if (ready_ && state_ == State::CANCEL_ALL) {
      (*this)(State::QUOTE);
    }
  }

  void operator()(Event<OrderAck> const &event) override {
    auto &[message_info, order_ack] = event;
    get_quote(order_ack.side)(event);
  }

  // XXX TODO update virtual position
  void operator()(Event<OrderUpdate> const &event) override {
    auto &[message_info, order_update] = event;
    get_quote(order_update.side)(event);
  }

  void operator()(Event<TradeUpdate> const &) override {}

  // real-time position feed

  void operator()(Event<PortfolioUpdate> const &) override {}

  // helpers

  void operator()(State state) {
    if (utils::update(state_, state)) {
      switch (state_) {
        using enum State;
        case UNDEFINED:
        case CANCEL_ALL:
          log::warn("*** CANCEL ALL ***"sv);
          break;
        case QUOTE:
          log::warn("*** START QUOTING ***"sv);
          break;
      }
    }
  }

  bool can_trade() const { return ready_ && state_ == State::QUOTE && !std::isnan(tick_size_) && trading_status_ == TradingStatus::OPEN; }

  void cancel_all_orders() const {
    auto cancel_all_orders = CancelAllOrders{
        .account = ACCOUNT,
        .order_id = {},
        .exchange = EXCHANGE,
        .symbol = SYMBOL,
        .strategy_id = {},
        .side = {},
    };
    log::info("cancel_all_orders={}"sv, cancel_all_orders);
    shared_.dispatcher.send(cancel_all_orders, 0);
  }

  void update_quotes() {
    if (std::isnan(mid_price_)) [[unlikely]] {
    } else {
      auto half_spread = 0.5 * SPREAD * tick_size_;
      auto skew = [&]() {
        if (utils::is_zero(shared_.position)) {
          return 0.0;
        }
        return shared_.position < 0.0 ? tick_size_ : -tick_size_;
      }();
      auto bid_price = market::round_away(Side::BUY, mid_price_ + skew - half_spread, tick_size_);
      auto ask_price = market::round_away(Side::SELL, mid_price_ + skew + half_spread, tick_size_);
      bid_.update_target_price(bid_price);
      ask_.update_target_price(ask_price);
    }
  }

  Quote &get_quote(Side side) {
    switch (side) {
      using enum Side;
      case UNDEFINED:
        break;
      case BUY:
        return bid_;
      case SELL:
        return ask_;
    }
    log::fatal("Unexpected"sv);
  }

 private:
  Shared shared_;
  bool ready_ = false;
  State state_ = {};
  uint64_t max_order_id_ = {};
  double tick_size_ = NaN;
  TradingStatus trading_status_ = {};
  double mid_price_ = NaN;
  Quote bid_;
  Quote ask_;
};

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &args) override {
    auto params = args.params();
    if (std::empty(params)) {
      log::fatal("Expected arguments"sv);
    }
    client::flags::Settings settings{args};
    Config config;
    client::Trader{settings, config, params}.dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};
}  // namespace maker
}  // namespace roq

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  roq::flags::Args args{argc, argv, INFO.description, INFO.build_version};
  roq::logging::flags::Settings settings{args};
  return roq::maker::Application{args, settings, INFO}.run();
}

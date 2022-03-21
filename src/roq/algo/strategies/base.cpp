/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/algo/strategies/base.hpp"

#include "roq/logging.hpp"

#include "roq/utils/common.hpp"
#include "roq/utils/compare.hpp"
#include "roq/utils/update.hpp"

using namespace std::literals;

namespace roq {
namespace algo {
namespace strategies {

namespace {
auto create_order_managers(Base &base, const framework::State &state, const CreateOrder &create_order) {
  std::vector<OrderManager> result;
  result.reserve(std::size(state));
  for (size_t index = 0; index < std::size(state); ++index) {
    auto &instrument = state.get_instrument(index);
    auto direction = utils::compare(instrument.weight, 0.0);
    switch (direction) {
      case 0:
        log::fatal("Weight can not be zero (index={})"sv, index);
        assert(false);
        break;
      case 1:
        result.emplace_back(base, index, create_order.side);
        break;
      case -1:
        result.emplace_back(base, index, utils::invert(create_order.side));
        break;
      default:
        assert(false);
    }
  }
  return result;
}
}  // namespace

Base::Base(
    framework::Dispatcher &dispatcher,
    const framework::State &state,
    const std::string_view &routing_id,
    const CreateOrder &create_order)
    : dispatcher_(dispatcher), state_(state), routing_id_(routing_id), account_(create_order.account),
      order_managers_(create_order_managers(*this, state_, create_order)), ready_by_instrument_(std::size(state_)) {
  assert(std::empty(create_order.routing_id));
}

void Base::operator()(const Event<Connected> &event) {
  update(event);
}

void Base::operator()(const Event<Disconnected> &event) {
  update(event);
}

void Base::operator()(const Event<DownloadBegin> &event) {
  update(event);
}

void Base::operator()(const Event<DownloadEnd> &event) {
  update(event);
}

void Base::operator()(const Event<GatewaySettings> &event) {
  update(event);
}

void Base::operator()(const Event<GatewayStatus> &event) {
  update(event);
}

void Base::operator()(const Event<ReferenceData> &event) {
  update(event);
}

void Base::operator()(const Event<MarketStatus> &event) {
  // XXX currently managed by the instrument -- maybe we need more control (like cancel-only)
  update(event);
}

void Base::operator()(const Event<OrderAck> &event) {
  dispatch(event);
}

void Base::operator()(const Event<OrderUpdate> &event) {
  dispatch(event);
}

// note! the cached state is already updated prior to this method being called
template <typename T>
void Base::update(const Event<T> &event) {
  auto ready = true;
  for (size_t index = 0; index < std::size(state_); ++index) {
    // note! vector<bool> doesn't support reference to item
    bool tmp = ready_by_instrument_[index];
    if (state_.ready(tmp, index, event.message_info.source, account_)) {
      log::debug("INSTRUMENT #{} is {}READY"sv, index, (tmp ? ""sv : "NOT "sv));
      ready_by_instrument_[index] = tmp;
    }
    ready &= tmp;
  }
  if (utils::update(ready_, ready)) {
    if (ready_) {
      (*this)(Ready{});
    } else {
      (*this)(NotReady{});
    }
  }
}

// note! the order managers will themselves maintain the lookup table
template <typename T>
void Base::dispatch(const Event<T> &event) {
  auto &[message_info, value] = event;
  auto iter = order_id_to_order_manager_index_.find(value.order_id);
  if (iter != std::end(order_id_to_order_manager_index_)) {
    auto index = (*iter).second;
    order_managers_[index](event);
  } else {
    log::fatal("Unknown order_id={}"sv, value.order_id);
  }
}

}  // namespace strategies
}  // namespace algo
}  // namespace roq

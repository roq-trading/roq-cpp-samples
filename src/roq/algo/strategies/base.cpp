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
auto create_order_managers(Base &base, framework::State const &state, CreateOrder const &create_order) {
  std::vector<OrderManager> result;
  result.reserve(std::size(state));
  for (size_t index = 0; index < std::size(state); ++index) {
    auto &instrument = state.get_instrument(index);
    auto ordering = utils::compare(instrument.weight, 0.0);
    if (ordering == std::strong_ordering::equal) {
      log::fatal("Weight can not be zero (index={})"sv, index);
      assert(false);
    } else if (ordering == std::strong_ordering::greater) {
      result.emplace_back(base, index, create_order.side);
    } else if (ordering == std::strong_ordering::less) {
      result.emplace_back(base, index, utils::invert(create_order.side));
    } else {
      assert(false);
    }
  }
  return result;
}
}  // namespace

Base::Base(
    framework::Dispatcher &dispatcher,
    framework::State const &state,
    std::string_view const &routing_id,
    CreateOrder const &create_order)
    : dispatcher_(dispatcher), state_(state), routing_id_(routing_id), account_(create_order.account),
      order_managers_(create_order_managers(*this, state_, create_order)), ready_by_instrument_(std::size(state_)) {
  assert(std::empty(create_order.routing_id));
}

void Base::operator()(Event<Connected> const &event) {
  update(event);
}

void Base::operator()(Event<Disconnected> const &event) {
  update(event);
}

void Base::operator()(Event<DownloadBegin> const &event) {
  update(event);
}

void Base::operator()(Event<DownloadEnd> const &event) {
  update(event);
}

void Base::operator()(Event<GatewaySettings> const &event) {
  update(event);
}

void Base::operator()(Event<GatewayStatus> const &event) {
  update(event);
}

void Base::operator()(Event<ReferenceData> const &event) {
  update(event);
}

void Base::operator()(Event<MarketStatus> const &event) {
  // XXX currently managed by the instrument -- maybe we need more control (like cancel-only)
  update(event);
}

void Base::operator()(Event<OrderAck> const &event) {
  dispatch(event);
}

void Base::operator()(Event<OrderUpdate> const &event) {
  dispatch(event);
}

// note! the cached state is already updated prior to this method being called
template <typename T>
void Base::update(Event<T> const &event) {
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
void Base::dispatch(Event<T> const &event) {
  auto &[message_info, value] = event;
  auto iter = order_id_to_order_manager_index_.find(value.order_id);
  if (iter != std::end(order_id_to_order_manager_index_)) {
    auto index = (*iter).second;
    order_managers_[index](event);
  } else {
    log::warn("Unknown order_id={}"sv, value.order_id);
  }
}

}  // namespace strategies
}  // namespace algo
}  // namespace roq

/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/python/strategy.hpp"

#include <string>
#include <utility>

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace python {

// === CONSTANTS ===

namespace {
auto const TIMER_FREQUENCY = 2s;
}

// === HELPERS ===

namespace {
auto create_module(auto const &name) {
  std::string name_2{name};  // note! pybind11 interface requires char const *
  return pybind11::module::import(name_2.c_str());
}

template <typename T, typename... Args>
auto call(auto &module, auto const &name, Args &&...args) {
  std::string name_2{name};  // note! pybind11 interface requires char const *
  auto res = module.attr(name_2.c_str())(std::forward<Args>(args)...);
  return res.template cast<T>();
}
}  // namespace

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_{dispatcher}, module_{create_module("foo"sv)} {
}

void Strategy::operator()(Event<Timer> const &event) {
  if (event.value.now < next_timer_) {
    return;
  }
  next_timer_ = event.value.now + TIMER_FREQUENCY;
  try {
    auto result = call<double>(module_, "bar"sv, layer_.bid_price, layer_.ask_price);
    log::debug("bid={}, ask={}, mid={}"sv, layer_.bid_price, layer_.ask_price, result);
  } catch (std::exception &e) {
    log::error(R"(type={}, what="{}")"sv, typeid(e).name(), e.what());
  }
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  log::info("[{}:{}] TopOfBook={}"sv, event.message_info.source, event.message_info.source_name, event.value);
  layer_ = event.value.layer;
}

}  // namespace python
}  // namespace samples
}  // namespace roq

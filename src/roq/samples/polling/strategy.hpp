/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>
#include <string_view>

#include "roq/api.hpp"

#include "roq/io/sys/signal.hpp"

#include "roq/client/simple.hpp"

#include "roq/samples/polling/config.hpp"
#include "roq/samples/polling/settings.hpp"

namespace roq {
namespace samples {
namespace polling {

// note!
//   we need a signal handler so the application can be terminated
//   signals are not handled by the polling client api because it "depends"
//   for this example, we need to install the signal handler
//   for other use-cases, perhaps the signal handler is handled elsewhere
//   keep this in mind before you implement your own version

struct Strategy final : public client::Simple::Handler, public io::sys::Signal::Handler {
  Strategy(Settings const &, Config const &, io::Context &, std::span<std::string_view const> const &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

  void dispatch();

 protected:
  enum class State {
    UNDEFINED,
    CONNECTING,
    READY,
    CREATE_ORDER,
    WAITING_CREATE,
    WORKING,
    CANCEL_ORDER,
    WAITING_CANCEL,
    DONE,
  };

  void operator()(State);

  void refresh(std::chrono::nanoseconds now);

  void create_order();
  void cancel_order();

  // client::Simple::Handler
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<Ready> const &) override;
  void operator()(Event<GatewaySettings> const &) override;
  void operator()(Event<StreamStatus> const &) override;
  void operator()(Event<GatewayStatus> const &) override;
  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;
  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;
  void operator()(Event<OrderAck> const &) override;
  void operator()(Event<OrderUpdate> const &) override;

  // io::sys::Signal::Handler
  void operator()(io::sys::Signal::Event const &) override;

 private:
  Settings const &settings_;
  std::unique_ptr<io::sys::Signal> terminate_;
  std::unique_ptr<io::sys::Signal> interrupt_;
  std::unique_ptr<client::Simple> dispatcher_;
  std::chrono::nanoseconds next_yield_ = {};
  State state_ = {};
  std::chrono::nanoseconds next_update_ = {};
  uint64_t max_order_id_ = {};
  uint64_t order_id_ = {};
};

}  // namespace polling
}  // namespace samples
}  // namespace roq

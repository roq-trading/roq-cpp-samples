/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>
#include <string_view>

#include "roq/api.hpp"

#include "roq/client/simple.hpp"

#include "roq/samples/polling/config.hpp"
#include "roq/samples/polling/settings.hpp"

namespace roq {
namespace samples {
namespace polling {

struct Strategy final : public client::Simple::Handler {
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
  void operator()(Event<OrderAck> const &) override;
  void operator()(Event<OrderUpdate> const &) override;

 private:
  Settings const &settings_;
  std::unique_ptr<client::Simple> dispatcher_;
  State state_ = {};
  std::chrono::nanoseconds next_update_ = {};
  uint64_t max_order_id_ = {};
  uint64_t order_id_ = {};
};

}  // namespace polling
}  // namespace samples
}  // namespace roq

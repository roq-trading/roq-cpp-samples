/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/collector/collector.h"

#include "gflags/gflags.h"

#include "roq/logging.h"
#include "roq/stream.h"

DEFINE_string(
    symbols,
    "",
    "symbols, one or more comma-separated lists prefixed "
    "with exchange, e.g. \"E1:S1,S2;E2:S3\")");

namespace roq {
namespace samples {
namespace collector {

namespace {
static const char *EXCHANGE = "CFFEX";
}  // namespace

Collector::Collector(roq::client::Dispatcher& dispatcher)
    : _dispatcher(dispatcher) {
}

void Collector::on(const roq::ConnectionStatusEvent& event) {
  auto source = event.source_name;
  switch (event.connection_status) {
    case ConnectionStatus::CONNECTED: {
      LOG(INFO) << "Connected to source=\"" << source << "\"";
      _dispatcher.send(
          Subscribe {
            .accounts = {},
            .symbols_by_exchange = { {
                EXCHANGE, { "*" }
              } }
          },
          event.source);
      break;
    }
    case ConnectionStatus::DISCONNECTED: {
      LOG(INFO) << "Disconnected from source=\"" << source << "\"";
      break;
    }
  }
}

void Collector::on(const roq::BatchBeginEvent&) {
}

void Collector::on(const roq::BatchEndEvent& event) {
}

void Collector::on(const roq::DownloadBeginEvent& event) {
  LOG_IF(FATAL, std::strlen(event.download_begin.account) > 0) <<
    "Unexpected";
  LOG(INFO) << "Downloading from "
    "source=\"" << event.message_info.source_name << "\" "
    "...";
  _download = true;
}

void Collector::on(const roq::DownloadEndEvent& event) {
  LOG_IF(FATAL, std::strlen(event.download_end.account) > 0) <<
    "Unexpected";
  LOG(INFO) << "Downloading from "
    "source=\"" << event.message_info.source_name << "\""
    ") has completed!";
  _download = false;
}

void Collector::on(const roq::ReferenceDataEvent& event) {
  LOG(INFO) << "ReferenceDataEvent=" << event;
}

void Collector::on(const roq::MarketByPriceEvent& event) {
  LOG(INFO) << "MarketByPriceEvent=" << event;
}

}  // namespace collector
}  // namespace samples
}  // namespace roq

/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/taker/performance.h"

namespace roq {
namespace samples {
namespace taker {

/*
void Performance::on(const ConnectionStatusEvent& event) {
}

void Performance::on(const BatchBeginEvent& event) {
}

void Performance::on(const BatchEndEvent& event) {
}
*/

void Performance::on(const MarketDataStatusEvent& event) {
}

void Performance::on(const OrderManagerStatusEvent& event) {
}

void Performance::on(const DownloadBeginEvent& event) {
}

void Performance::on(const DownloadEndEvent& event) {
}

void Performance::on(const ReferenceDataEvent& event) {
}

void Performance::on(const MarketStatusEvent& event) {
}

void Performance::on(const SessionStatisticsEvent& event) {
}

void Performance::on(const DailyStatisticsEvent& event) {
}

void Performance::on(const MarketByPriceEvent& event) {
}

void Performance::on(const TradeSummaryEvent& event) {
}

void Performance::on(const PositionUpdateEvent& event) {
}

void Performance::on(const OrderUpdateEvent& event) {
}

void Performance::on(const TradeUpdateEvent& event) {
}

void Performance::on(const CreateOrderEvent& event) {
}

void Performance::on(const ModifyOrderEvent& event) {
}

void Performance::on(const CancelOrderEvent& event) {
}

void Performance::on(const CreateOrderAckEvent& event) {
}

void Performance::on(const ModifyOrderAckEvent& event) {
}

void Performance::on(const CancelOrderAckEvent& event) {
}

/*
void Performance::write(Metrics& metrics) const {
}
*/

void Performance::extract(
    const std::string& type,
    const std::string& exchange,
    const std::string& symbol,
    client::DataFrame::Handler& handler) const {
}

}  // namespace taker
}  // namespace samples
}  // namespace roq

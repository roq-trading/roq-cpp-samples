/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <roq/simulation.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace examples {
namespace utilities {

class Matcher final : public roq::simulation::Matcher {
 public:
  typedef std::vector<roq::Layer> InstrumentOrderBook;
  typedef std::unordered_map<std::string, InstrumentOrderBook> OrderBook;

  Matcher(
      roq::simulation::Matcher::Dispatcher& dispatcher,
      const std::string& name);
  void on(const roq::BatchBeginEvent& event) override;
  void on(const roq::BatchEndEvent& event) override;
  void on(const roq::SessionStatisticsEvent& event) override;
  void on(const roq::DailyStatisticsEvent& event) override;
  void on(const roq::MarketByPriceEvent& event) override;
  void on(const roq::TradeSummaryEvent& event) override;
  void on(const roq::CreateOrder& create_order) override;
  void on(const roq::ModifyOrder& modify_order) override;
  void on(const roq::CancelOrder& cancel_order) override;

 private:
  OrderBook _order_book;
  // TODO(thraneh): drop this work-around when moving to C++17
  // SO18837857
  struct EnumHash {
    template <typename T>
    std::size_t operator()(T t) const {
      return static_cast<std::size_t>(t);
    }
  };
  std::unordered_map<std::string,                   // account
    std::unordered_map<std::string,                 // exchange
      std::unordered_map<std::string,               // symbol
        std::unordered_map<roq::Side, double, EnumHash> > > > _positions;
};

}  // namespace utilities
}  // namespace examples

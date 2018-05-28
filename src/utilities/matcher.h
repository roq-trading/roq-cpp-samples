/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/simulation.h>

#include <string>

namespace examples {
namespace utilities {

class Matcher final : public roq::simulation::Matcher {
 public:
  Matcher(
      roq::simulation::Matcher::Dispatcher& dispatcher,
      const std::string& name);
  void on(const roq::BatchBeginEvent& event) override;
  void on(const roq::BatchEndEvent& event) override;
  void on(const roq::MarketByPriceEvent& event) override;
  void on(const roq::TradeSummaryEvent& event) override;
  void on(const roq::CreateOrder& create_order) override;
  void on(const roq::ModifyOrder& modify_order) override;
  void on(const roq::CancelOrder& cancel_order) override;
};

}  // namespace utilities
}  // namespace examples

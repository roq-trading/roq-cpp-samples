/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_7 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<TopOfBook> const &) override;

 private:
  client::Dispatcher &dispatcher_;
  std::array<Measurement, 2> measurements_;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq

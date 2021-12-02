/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_7 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

 protected:
  void operator()(const Event<TopOfBook> &) override;

 private:
  client::Dispatcher &dispatcher_;
  std::array<Measurement, 2> measurements_;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq

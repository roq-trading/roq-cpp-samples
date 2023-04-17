/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <array>

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_7 {

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<TopOfBook> const &) override;

 private:
  client::Dispatcher &dispatcher_;
  std::array<Measurement, 2> measurements_;
  std::array<MatrixKey, 3> rows_;
  std::array<double, 3> data_;
  uint32_t version_ = {};
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq

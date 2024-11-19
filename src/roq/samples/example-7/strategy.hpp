/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <array>

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/example-7/settings.hpp"

namespace roq {
namespace samples {
namespace example_7 {

struct Strategy final : public client::Handler {
  Strategy(client::Dispatcher &, Settings const &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<TopOfBook> const &) override;

  void operator()(Event<CustomMetricsUpdate> const &) override;
  void operator()(Event<CustomMatrixUpdate> const &) override;

 private:
  client::Dispatcher &dispatcher_;
  Settings const &settings_;
  std::array<Measurement, 2> measurements_;
  std::array<MatrixKey, 3> rows_;
  std::array<double, 3> data_;
  uint32_t version_ = {};
  bool downloading_ = false;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq

/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <map>
#include <set>
#include <string>

#include "roq/client.hpp"

#include "roq/samples/experiment/settings.hpp"

namespace roq {
namespace samples {
namespace experiment {

struct Config final : public client::Config {
  explicit Config(Settings const &);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &) const override;

 private:
  std::set<std::string> const accounts_;
  std::map<std::string, std::set<std::string>> const exchange_symbols_;
};

}  // namespace experiment
}  // namespace samples
}  // namespace roq

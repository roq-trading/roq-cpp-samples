/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/samples/import/flags/flags.hpp"

namespace roq {
namespace samples {
namespace import {

struct Settings final : public flags::Flags {
  Settings();
};

}  // namespace import
}  // namespace samples
}  // namespace roq

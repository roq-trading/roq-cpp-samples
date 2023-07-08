/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/io-context/settings.hpp"

namespace roq {
namespace samples {
namespace io_context {

Settings::Settings() : flags::Flags{flags::Flags::create()} {
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq

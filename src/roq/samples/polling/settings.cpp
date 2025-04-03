/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/polling/settings.hpp"

namespace roq {
namespace samples {
namespace polling {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace polling
}  // namespace samples
}  // namespace roq

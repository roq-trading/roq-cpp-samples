/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/io-context/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace io_context {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq

/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <fstream>
#include <string_view>

#include "roq/api.hpp"

#include "roq/codec/flatbuffers/encoder.hpp"

#include "roq/samples/import/settings.hpp"

namespace roq {
namespace samples {
namespace import {

struct Processor final {
  Processor(Settings const &, std::string_view const &path);

  Processor(Processor &&) noexcept = default;
  Processor(Processor const &) = delete;

  ~Processor();

  void dispatch();

 protected:
  MessageInfo create_message_info(std::chrono::nanoseconds timestamp_utc);

  template <typename T>
  void process(T const &value, std::chrono::nanoseconds timestamp_utc);

 private:
  uint64_t seqno_ = {};
  std::unique_ptr<codec::flatbuffers::Encoder> encoder_;
  std::ofstream file_;
  enum class Encoding {
    BINARY,
    BASE64,
  } encoding_ = Encoding::BINARY;
};

}  // namespace import
}  // namespace samples
}  // namespace roq

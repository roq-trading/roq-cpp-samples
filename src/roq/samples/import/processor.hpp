/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <flatbuffers/flatbuffers.h>

#include <chrono>
#include <fstream>
#include <string_view>

#include "roq/api.hpp"

namespace roq {
namespace samples {
namespace import {

class Processor final {
 public:
  explicit Processor(std::string_view const &path);

  Processor(Processor &&) = default;
  Processor(Processor const &) = delete;

  ~Processor();

  void dispatch();

 protected:
  MessageInfo create_message_info(std::chrono::nanoseconds timestamp_utc);

  template <typename T>
  void process(const T &value, std::chrono::nanoseconds timestamp_utc);

 private:
  uint64_t seqno_ = {};
  flatbuffers::FlatBufferBuilder builder_;
  std::ofstream file_;
  enum class Encoding {
    BINARY,
    BASE64,
  } encoding_ = Encoding::BINARY;
};

}  // namespace import
}  // namespace samples
}  // namespace roq

/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/samples/common/tokenizer.h"

using namespace roq::samples::common;  // NOLINT

TEST(tokenizer, empty) {
  auto result = tokenize("", ",");
  EXPECT_EQ(std::size(result), size_t {0});
}

TEST(tokenizer, single) {
  auto result = tokenize("abc123", ",");
  ASSERT_EQ(std::size(result), size_t {1});
  ASSERT_EQ(std::size(result[0]), size_t {6});
  EXPECT_EQ(result[0], "abc123");
}

TEST(tokenizer, trivial) {
  auto result = tokenize("abc123,def", ",");
  ASSERT_EQ(std::size(result), size_t {2});
  ASSERT_EQ(std::size(result[0]), size_t {6});
  EXPECT_EQ(result[0], "abc123");
  ASSERT_EQ(std::size(result[1]), size_t {3});
  EXPECT_EQ(result[1], "def");
}

TEST(tokenizer, empty_tokens) {
  auto result = tokenize(",", ",");
  ASSERT_EQ(std::size(result), size_t {2});
  EXPECT_EQ(std::size(result[0]), size_t {0});
  EXPECT_EQ(std::size(result[1]), size_t {0});
}

TEST(tokenizer, some_empty_tokens) {
  auto result = tokenize("abc,,def,,", ",");
  ASSERT_EQ(std::size(result), size_t {5});
  ASSERT_GT(std::size(result[0]), size_t {0});
  EXPECT_EQ(result[0], "abc");
  EXPECT_EQ(std::size(result[1]), size_t {0});
  ASSERT_GT(std::size(result[2]), size_t {0});
  EXPECT_EQ(result[2], "def");
  EXPECT_EQ(std::size(result[3]), size_t {0});
  EXPECT_EQ(std::size(result[4]), size_t {0});
}

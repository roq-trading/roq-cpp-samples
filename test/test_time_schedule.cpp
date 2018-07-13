/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <cctz/civil_time.h>
#include <cctz/time_zone.h>

#include "utilities/time_schedule.h"

using namespace examples::utilities;  // NOLINT

const char *TIME_ZONE = "Asia/Shanghai";

namespace {
static std::chrono::system_clock::time_point create_time_point(
    cctz::time_zone& time_zone,
    uint8_t hour,
    uint8_t minute) {
  cctz::civil_second local_time(2018, 7, 13, hour, minute);
  return cctz::convert(local_time, time_zone);
}
}  // namespace

TEST(timer, simple) {
  TimeSchedule time_schedule(TIME_ZONE, { "12:00", "13:00", "14:00" });
  cctz::time_zone time_zone;
  ASSERT_TRUE(cctz::load_time_zone(TIME_ZONE, &time_zone));
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 11, 0)),
      0);
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 11, 59)),
      0);
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 12, 0)),
      1);
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 12, 59)),
      1);
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 13, 0)),
      2);
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 13, 59)),
      2);
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 14, 0)),
      3);
  EXPECT_EQ(
      time_schedule.get_index(create_time_point(time_zone, 23, 59)),
      3);
}

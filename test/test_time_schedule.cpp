/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "utilities/time_schedule.h"

using namespace examples::utilities;  // NOLINT

TEST(timer, simple) {
  Timer timer("Asia/Shanghai", { "12:00", "13:00", "14:00" }, "%H:%M");
}

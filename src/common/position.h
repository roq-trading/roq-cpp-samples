/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <limits>
#include <ostream>

namespace examples {
namespace common {

enum class PositionType {
  StartOfDay,   // today's start of day position
  NewActivity,  // today's new activity
  Current,      // current position
};

class Position final {
 public:
  explicit Position(
      double position = std::numeric_limits<double>::quiet_NaN());
  void reset();
  double get(PositionType type) const;
  void set_start_of_day(double position);
  void add_new_activity(double quantity);
  std::ostream& write(std::ostream& stream) const;

 private:
  const double _manual;
  const bool _use_position_update;
  double _start_of_day;
  double _new_activity = 0.0;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Position& position) {
  return position.write(stream);
}

}  // namespace common
}  // namespace examples

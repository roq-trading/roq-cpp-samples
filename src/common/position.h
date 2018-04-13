/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <ostream>

namespace examples {
namespace common {

enum class PositionType {
  StartOfDay,   // today's start of day position
  NewActivity,  // today's new activity
  Current,      // current position
  Reference,    // position when downloaded (debug)
};

class Position final {
 public:
  void reset();
  double get(PositionType type) const;
  void set_start_of_day(double position);
  void add_new_activity(double quantity);
  void set_reference(double position);

 private:
  double _start_of_day = 0.0;
  double _new_activity = 0.0;
  double _reference = 0.0;  // debug
};

std::ostream& operator<<(std::ostream&, const Position&);

}  // namespace common
}  // namespace examples

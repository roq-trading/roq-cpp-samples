/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <limits>
#include <ostream>

namespace examples {
namespace common {

enum class PositionType {
  // status
  StartOfDay,  // today's start of day position
  Closed,      // today's close activity
  Opened,      // today's open activity
  // derived
  Current,     // "net" open position
};

class Position final {
 public:
  explicit Position(
      double position = std::numeric_limits<double>::quiet_NaN());
  void reset();
  bool can_close() const;
  double get(PositionType type) const;
  void on(const roq::PositionUpdate& position_update);
  void open(uint32_t order_id, double quantity);
  void close(uint32_t order_id, double quantity);
  std::ostream& write(std::ostream& stream) const;

 private:
  const double _manual;
  const bool _use_position_update;
  uint32_t _last_order_id;
  double _start_of_day;
  double _closed = 0.0;
  double _opened = 0.0;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Position& position) {
  return position.write(stream);
}

}  // namespace common
}  // namespace examples

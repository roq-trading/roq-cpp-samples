/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/options.h"

#include <cstdint>
#include <limits>

namespace roq {
namespace samples {
namespace example_3 {

class EMA final {
 public:
  EMA(double alpha);

  EMA(const EMA&) = delete;
  EMA(EMA&&) = default;

  operator double() const {
    return _value;
  }

  void reset();

  bool is_ready() const {
    return _countdown == 0;
  }

  double update(double value);

 private:
  const double _alpha;
  double _value = std::numeric_limits<double>::quiet_NaN();
  uint32_t _countdown = FLAGS_warmup;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq

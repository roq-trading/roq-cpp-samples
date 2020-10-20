/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <string_view>

#include "roq/service.h"

#include "roq/span.h"

namespace roq {
namespace samples {
namespace example_3 {

class Controller final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const roq::span<std::string_view> &args);

  int main(int argc, char **argv) override;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq

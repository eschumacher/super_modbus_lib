
#pragma once

#include <cstdint>

namespace supermodbus {

struct MBAddressSpan {
  uint16_t first_address_{0};
  uint16_t reg_count_{0};
};

}  // namespace supermodbus

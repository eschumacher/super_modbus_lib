
#pragma once

#include <iostream>
#include "mb_address_span.hpp"
#include "mb_function_code.hpp"

namespace supermodbus {

constexpr uint8_t kMaxSlaveID{247};

class MBRequest {
 public:
  MBRequest(uint8_t slave_id, MBFunctionCode fn_code, MBAddressSpan addr_span)
      : slave_id_{slave_id},
        fn_code_{fn_code},
        addr_span_{addr_span} {}

  [[nodiscard]] bool is_valid() const {
    return fn_code_.is_valid() && slave_id_ <= kMaxSlaveID;
  }

  void print() const {
    std::cout << "MBRequest: " << is_valid() << ", " << fn_code_.get_code()
              << std::endl;
  }

 private:
  uint8_t slave_id_{};
  MBFunctionCode fn_code_{};
  MBAddressSpan addr_span_{};
};

}  // namespace supermodbus

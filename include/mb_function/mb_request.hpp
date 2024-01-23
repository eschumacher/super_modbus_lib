
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
    std::cout << "MBRequest:\n";
    std::cout << "\t" << (is_valid() ? "Valid" : "Invalid") << "\n";
    std::cout << "\tFunction Code: " << fn_code_.get_code() << "\n";
    std::cout << "\tStart Address: " << addr_span_.first_address_ << "\n";
    std::cout << "\tNum Registers: " << addr_span_.reg_count_ << std::endl;
  }

 private:
  uint8_t slave_id_{};
  MBFunctionCode fn_code_{};
  MBAddressSpan addr_span_{};
};

}  // namespace supermodbus

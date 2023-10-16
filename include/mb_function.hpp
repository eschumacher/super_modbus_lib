
#include <array>
#include <cstdint>
#include <iostream>
#include <optional>

namespace supermodbus {

// TODO: uncomment class
enum /*class*/ FnCode {
  // In Decimal
  // If changed, check if MBFunctionCode(uint8_t) constructor impacted
  kUnsupported = -1,
  kInvalid = 0,
  kReadCoils = 1,
  kReadDI = 2,
  kReadHR = 3,
  kReadIR = 4,
  kWriteSingleCoil = 5,
  kWriteSingleReg = 6,
  kReadExceptionStatus = 7,
  kDiagnostics = 8,
  kGetComEventCounter = 11,
  kGetComEventLog = 12,
  kWriteMultCoils = 15,
  kWriteMultRegs = 16,
  kReportSlaveID = 17,
  kReadFileRecord = 20,
  kWriteFileRecord = 21,
  kMaskWriteReg = 22,
  kReadWriteMultRegs = 23,
  kReadFIFOQueue = 24
};

class MBFunctionCode {
 public:
  explicit MBFunctionCode(FnCode code)
      : code_{code} {}

  explicit MBFunctionCode(uint8_t const &code) {
    // Must maintain this logic for fn code gaps if FnCode enum definition
    // changes
    if ((code > FnCode::kInvalid && code <= FnCode::kWriteSingleReg) ||
        code == FnCode::kWriteMultRegs) {
      code_ = static_cast<FnCode>(code);
    }
  }

  [[nodiscard]] bool is_valid() const noexcept { return code_ != kInvalid; }

  [[nodiscard]] FnCode get_code() const noexcept { return code_; }

 private:
  FnCode code_{FnCode::kInvalid};
};

struct MBAddressSpan {
  uint16_t first_address_{0};
  uint16_t reg_count_{0};
};

class MBRequest {
 public:
  MBRequest(MBFunctionCode fn_code, MBAddressSpan addr_span)
      : fn_code_{fn_code},
        addr_span_{addr_span} {}

  [[nodiscard]] bool is_valid() const { return fn_code_.is_valid(); }

  void print() {
    std::cout << "MBRequest: " << is_valid() << ", " << fn_code_.get_code()
              << std::endl;
  }

 private:
  MBFunctionCode fn_code_{kInvalid};
  MBAddressSpan addr_span_{};
};

template <typename Iter>
[[nodiscard]] std::optional<MBRequest> parse_req_from_bytes(Iter begin,
                                                            Iter end) {
  MBRequest mb_req{MBFunctionCode{*begin}, {*(begin + 1), *(begin + 3)}};
  if (mb_req.is_valid()) {
    return mb_req;
  }

  return {};
}

}  // namespace supermodbus

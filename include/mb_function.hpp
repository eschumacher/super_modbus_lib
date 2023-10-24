
#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <span>

namespace supermodbus {

class MBFunctionCode {
 public:
  enum FnCode {
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
    kReadFIFOQueue = 24,
    kMax = kReadFIFOQueue
  };

  MBFunctionCode() = default;

  explicit MBFunctionCode(FnCode code) {
    if (code == kInvalid || code > kMax) {
      code_ = kInvalid;
    } else {
      switch (code) {
        // TODO: when adding support for a FnCode, handle here
        case kReadHR: {
          code_ = static_cast<FnCode>(code);
          break;
        }
        case kReadCoils:
        case kReadDI:
        case kReadIR:
        case kWriteSingleCoil:
        case kWriteSingleReg:
        case kReadExceptionStatus:
        case kDiagnostics:
        case kGetComEventCounter:
        case kGetComEventLog:
        case kWriteMultCoils:
        case kWriteMultRegs:
        case kReportSlaveID:
        case kReadFileRecord:
        case kWriteFileRecord:
        case kMaskWriteReg:
        case kReadWriteMultRegs:
        case kReadFIFOQueue:
        default: {
          code_ = kUnsupported;
          break;
        }
      }
    }
  }

  explicit MBFunctionCode(uint8_t const &code)
      : MBFunctionCode{static_cast<FnCode>(code)} {}

  [[nodiscard]] bool is_valid() const noexcept {
    return code_ != kInvalid && code_ != kUnsupported;
  }

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

  void print() const {
    std::cout << "MBRequest: " << is_valid() << ", " << fn_code_.get_code()
              << std::endl;
  }

 private:
  MBFunctionCode fn_code_{};
  MBAddressSpan addr_span_{};
};

[[nodiscard]] std::optional<MBRequest> parse_req_from_bytes(auto &&bytes) {
  MBRequest mb_req{MBFunctionCode{bytes[1]}, {bytes[2], bytes[4]}};
  if (mb_req.is_valid()) {
    return mb_req;
  }

  mb_req.print();

  return {};
}

}  // namespace supermodbus

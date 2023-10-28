
#pragma once

#include <cstdint>

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
        /**
         * SUPPORTED FUNCTIONS
         * When adding support for a FnCode, handle here
         */
        case kReadHR:
        case kReadIR: {
          code_ = static_cast<FnCode>(code);
          break;
        }
        /**
         * UNSUPPORTED FUNCTIONS
         */
        case kReadCoils:
        case kReadDI:
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

}  // namespace supermodbus


#pragma once

#include <endian.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <span>
#include <stdexcept>

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

constexpr uint8_t kRTUReqIndexSlaveID{0};
constexpr uint8_t kRTUReqIndexFunctionCode{1};
constexpr uint8_t kRTUReqIndexStartAddress{2};
constexpr uint8_t kRTUReqIndexRegCount{4};
constexpr uint8_t kRTUReqIndexCRC{6};
constexpr uint8_t kRTUReqTotalBytes{8};
constexpr uint8_t kMaxNumRegs{125};

uint16_t span_to_uint16_t(auto &&span, std::size_t offset = 0) {
  if (offset + 1 >= span.size()) {
    throw std::out_of_range("Span does not have enough data.");
  }

  uint16_t result =
      (static_cast<uint16_t>(span[offset]) << 8) | span[offset + 1];

  return result;
}

[[nodiscard]] std::optional<MBRequest> parse_req_from_bytes(auto &&bytes) {
  if (sizeof(bytes) != kRTUReqTotalBytes) {
    std::cerr << "Invalid number of bytes!\n";
    return {};
  }

  uint8_t slave_id{bytes[kRTUReqIndexSlaveID]};
  MBFunctionCode fn_code{bytes[kRTUReqIndexFunctionCode]};
  uint16_t start_addr =
      be16toh(span_to_uint16_t(bytes, kRTUReqIndexStartAddress));
  uint16_t num_regs = be16toh(span_to_uint16_t(bytes, kRTUReqIndexRegCount));
  MBAddressSpan addr_span{start_addr, num_regs};
  uint16_t crc{be16toh(span_to_uint16_t(bytes, kRTUReqIndexCRC))};

  if (num_regs > kMaxNumRegs) {
    std::cerr << "Exceed max number of registers!\n";
    return {};
  }

  if (static_cast<uint32_t>(num_regs) + start_addr >
      std::numeric_limits<uint16_t>::max()) {
    std::cerr << "Boundary exception: requested registers exceeds 65535!\n";
    return {};
  }

  // TODO: check CRC

  MBRequest mb_req{fn_code, addr_span};
  if (mb_req.is_valid()) {
    return mb_req;
  }

  mb_req.print();

  return {};
}

}  // namespace supermodbus

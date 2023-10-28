
#pragma once

#include <endian.h>
#include <array>
#include <cstdint>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <mb_crc16.hpp>
#include <optional>
#include <span>
#include <stdexcept>
#include "endian_helpers.hpp"

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

constexpr uint8_t kRTUReqIndexSlaveID{0};
constexpr uint8_t kRTUReqIndexFunctionCode{1};
constexpr uint8_t kRTUReqIndexStartAddress{2};
constexpr uint8_t kRTUReqIndexRegCount{4};
constexpr uint8_t kRTUReqIndexCRC{6};
constexpr uint8_t kRTUReqTotalBytes{8};
constexpr uint8_t kMaxNumRegs{125};

[[nodiscard]] inline std::optional<MBRequest> parse_req_from_bytes(
    std::span<uint8_t const> bytes) {
  if (bytes.size() != kRTUReqTotalBytes) {
    std::cerr << "Invalid number of bytes!\n";
    return {};
  }

  uint8_t slave_id{bytes[kRTUReqIndexSlaveID]};
  MBFunctionCode fn_code{bytes[kRTUReqIndexFunctionCode]};
  uint16_t start_addr{
      from_big_endian<uint16_t>(bytes, kRTUReqIndexStartAddress)};
  uint16_t num_regs{from_big_endian<uint16_t>(bytes, kRTUReqIndexRegCount)};
  MBAddressSpan addr_span{start_addr, num_regs};
  uint16_t crc{from_big_endian<uint16_t>(bytes, kRTUReqIndexCRC)};

  if (num_regs > kMaxNumRegs) {
    std::cerr << "Exceed max number of registers!\n";
    return {};
  }

  if (num_regs == 0) {
    std::cerr << "Must request at least 1 register!\n";
    return {};
  }

  if (static_cast<uint32_t>(num_regs - 1) + start_addr >
      std::numeric_limits<uint16_t>::max()) {
    std::cerr << "Boundary exception: requested registers exceeds 65535!\n";
    return {};
  }

  uint16_t crc_check = htobe16(mb_crc16(bytes.subspan(0, 6)));
  if (crc_check != crc) {
    std::cerr << "Invalid CRC! Received: " << crc << ", expected: " << crc_check
              << "\n";
    return {};
  }

  MBRequest mb_req{slave_id, fn_code, addr_span};
  if (mb_req.is_valid()) {
    return mb_req;
  }

  mb_req.print();

  return {};
}

}  // namespace supermodbus

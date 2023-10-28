
#include "mb_function.hpp"
#include "endian_helpers.hpp"
#include "mb_crc16.hpp"
#include "mb_function/mb_address_span.hpp"
#include "mb_function/mb_function_code.hpp"

namespace supermodbus {

[[nodiscard]] std::optional<MBRequest> parse_req_from_bytes(
    std::span<uint8_t const> bytes) {
  // TODO: refactor to support various types of modbus request function codes
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

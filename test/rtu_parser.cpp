
#include <endian.h>
#include <gtest/gtest.h>
#include <algorithm>
#include "mb_function.hpp"

namespace supermodbus {

constexpr size_t kRTUReadRequestBytes = 8;
using RTUReadRequest = std::array<uint8_t, kRTUReadRequestBytes>;

uint8_t high_byte(uint16_t value) { return (value >> 8) & 0xFF; }
uint8_t low_byte(uint16_t value) { return value & 0xFF; }

TEST(RTUParser, ReadHR) {
  uint8_t slave_id = 1;
  uint8_t fn_code = FnCode::kReadHR;
  uint16_t address = htobe16(3001);
  uint16_t reg_count = htobe16(1);
  uint16_t crc = htobe16(52055);
  RTUReadRequest raw_req{};
  raw_req[0] = slave_id;
  raw_req[1] = fn_code;
  raw_req[2] = high_byte(address);
  raw_req[3] = low_byte(address);
  raw_req[4] = high_byte(reg_count);
  raw_req[5] = low_byte(reg_count);
  raw_req[6] = high_byte(crc);
  raw_req[7] = low_byte(crc);

  auto test_req{
      supermodbus::parse_req_from_bytes(raw_req.cbegin(), raw_req.cend())};

  EXPECT_TRUE(test_req);

  EXPECT_EQ(0x01, raw_req[4]);
  EXPECT_EQ(0x00, raw_req[5]);
}

}  // namespace supermodbus


#include <gtest/gtest.h>
#include <array>
#include <mb_crc16.hpp>
#include <mb_function.hpp>
#include "rtu_parser_general.hpp"

namespace {

TEST(RTUParserReadCoil, Valid) {
  constexpr RawReadRequest kValidRequest{0x01, 0x01, 0x03, 0xE9,
                                         0x00, 0x01, 0x2C, 0x7A};

  parse_and_check(kValidRequest, true);
}

TEST(RTUParserReadCoil, AddressBounds) {
  constexpr RawReadRequest kMinAddress{0x01, 0x01, 0x00, 0x00,
                                       0x00, 0x01, 0xFD, 0xCA};
  constexpr RawReadRequest kMaxAddress{0x01, 0x01, 0xFF, 0xFF,
                                       0x00, 0x01, 0xFD, 0xEE};

  parse_and_check(kMinAddress, true);
  parse_and_check(kMaxAddress, true);
}

TEST(RTUParserReadCoil, QuantityBounds) {
  constexpr RawReadRequest kTooLittle{0x01, 0x01, 0x03, 0xE9,
                                      0x00, 0x00, 0xED, 0xBA};
  constexpr RawReadRequest kTooMany{0x01, 0x01, 0x03, 0xE9,
                                    0x07, 0xD1, 0x2F, 0xD6};
  constexpr RawReadRequest kInvalidAddressBoundary{0x01, 0x01, 0xF8, 0x30,
                                                   0x07, 0xD0, 0x0E, 0xC9};
  constexpr RawReadRequest kValidAddressBoundary{0x01, 0x01, 0xF8, 0x2F,
                                                 0x07, 0xD0, 0x3F, 0x0F};

  parse_and_check(kTooLittle, false);
  parse_and_check(kTooMany, false);
  parse_and_check(kInvalidAddressBoundary, false);
  parse_and_check(kValidAddressBoundary, true);
}

TEST(RTUParserReadHR, SlaveAddressBounds) {
  constexpr RawReadRequest kInvalidSlaveAddr{0xF8, 0x01, 0x03, 0xE9,
                                             0x00, 0x01, 0x38, 0x13};
  constexpr RawReadRequest kMaxSlaveAddr{0xF7, 0x01, 0x03, 0xE9,
                                         0x00, 0x01, 0x38, 0xEC};

  parse_and_check(kInvalidSlaveAddr, false);
  parse_and_check(kMaxSlaveAddr, true);
}

TEST(RTUParserReadHR, CRC) {
  constexpr RawReadRequest kInvalidCRC{0x01, 0x01, 0x03, 0xE9,
                                       0x00, 0x01, 0x7A, 0x2C};
  constexpr RawReadRequest kValidCRC{0x01, 0x01, 0x03, 0xE9,
                                     0x00, 0x01, 0x2C, 0x7A};

  parse_and_check(kInvalidCRC, false);
  parse_and_check(kValidCRC, true);
}

TEST(RTUParserReadHR, TooLittleBytes) {
  constexpr std::array<uint8_t, 7> kShortRequest{0x01, 0x01, 0x03, 0xE9,
                                                 0x00, 0x01, 0x7A};

  parse_and_check(std::span(kShortRequest), false);
}

TEST(RTUParserReadHR, TooManyBytes) {
  constexpr std::array<uint8_t, 9> kLongRequest{0x01, 0x01, 0x03, 0xE9, 0x00,
                                                0x01, 0x2C, 0x7A, 0x01};

  parse_and_check(kLongRequest, false);
}

}  // namespace

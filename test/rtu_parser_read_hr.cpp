
#include <gtest/gtest.h>
#include <array>
#include <mb_crc16.hpp>
#include <mb_function.hpp>
#include "rtu_parser_general.hpp"

namespace {

TEST(RTUParserReadHR, Valid) {
  constexpr RawReadRequest kValidRequest{0x01, 0x03, 0x0B, 0xB9,
                                         0x00, 0x01, 0x57, 0xCB};

  parse_and_check(kValidRequest, true);
}

TEST(RTUParserReadHR, AddressBounds) {
  constexpr RawReadRequest kMinAddress{0x01, 0x03, 0x00, 0x00,
                                       0x00, 0x01, 0x84, 0x0A};
  constexpr RawReadRequest kMaxAddress{0x01, 0x03, 0xFF, 0xFF,
                                       0x00, 0x01, 0x84, 0x2E};

  parse_and_check(kMinAddress, true);
  parse_and_check(kMaxAddress, true);
}

TEST(RTUParserReadHR, QuantityBounds) {
  constexpr RawReadRequest kTooLittle{0x01, 0x03, 0x0B, 0xB9,
                                      0x00, 0x00, 0x96, 0x0B};
  constexpr RawReadRequest kTooMany{0x01, 0x03, 0x0B, 0xB9,
                                    0x00, 0x7E, 0x16, 0x2B};
  constexpr RawReadRequest kInvalidAddressBoundary{0x01, 0x03, 0xFF, 0x84,
                                                   0x00, 0x7D, 0xF5, 0xD6};
  constexpr RawReadRequest kValidAddressBoundary{0x01, 0x03, 0xFF, 0x83,
                                                 0x00, 0x7D, 0x44, 0x17};

  parse_and_check(kTooLittle, false);
  parse_and_check(kTooMany, false);
  parse_and_check(kInvalidAddressBoundary, false);
  parse_and_check(kValidAddressBoundary, true);
}

TEST(RTUParserReadHR, SlaveAddressBounds) {
  constexpr RawReadRequest kInvalidSlaveAddr{0xF8, 0x03, 0x0B, 0xB9,
                                             0x00, 0x01, 0x43, 0xA2};
  constexpr RawReadRequest kMaxSlaveAddr{0xF7, 0x03, 0x0B, 0xB9,
                                         0x00, 0x01, 0x43, 0x5D};

  parse_and_check(kInvalidSlaveAddr, false);
  parse_and_check(kMaxSlaveAddr, true);
}

TEST(RTUParserReadHR, CRC) {
  constexpr RawReadRequest kInvalidCRC{0x01, 0x03, 0x0B, 0xB9,
                                       0x00, 0x01, 0xCB, 0x57};
  constexpr RawReadRequest kValidCRC{0x01, 0x03, 0x0B, 0xB9,
                                     0x00, 0x01, 0x57, 0xCB};

  parse_and_check(kInvalidCRC, false);
  parse_and_check(kValidCRC, true);
}

TEST(RTUParserReadHR, TooLittleBytes) {
  constexpr std::array<uint8_t, 7> kShortRequest{0x01, 0x03, 0x0B, 0xB9,
                                                 0x00, 0x01, 0x57};

  parse_and_check(std::span(kShortRequest), false);
}

TEST(RTUParserReadHR, TooManyBytes) {
  constexpr std::array<uint8_t, 9> kLongRequest{0x01, 0x03, 0x0B, 0xB9, 0x00,
                                                0x01, 0x57, 0xCB, 0x01};

  parse_and_check(kLongRequest, false);
}

}  // namespace

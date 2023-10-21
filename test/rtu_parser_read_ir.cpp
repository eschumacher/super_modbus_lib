
#include <gtest/gtest.h>
#include <array>
#include <mb_function.hpp>
#include "rtu_parser_general.hpp"

namespace {

TEST(RTUParserReadIR, Valid) {
  constexpr RawReadRequest kValidRequest{0x01, 0x04, 0x0F, 0xA1,
                                         0x00, 0x01, 0x63, 0x3C};

  parse_and_check(kValidRequest, true);
}

TEST(RTUParserReadIR, AddressBounds) {
  constexpr RawReadRequest kMinAddress{0x01, 0x04, 0x00, 0x00,
                                       0x00, 0x01, 0x31, 0xCA};
  constexpr RawReadRequest kMaxAddress{0x01, 0x04, 0xFF, 0xFF,
                                       0x00, 0x01, 0x31, 0xEE};

  parse_and_check(kMinAddress, true);
  parse_and_check(kMaxAddress, true);
}

TEST(RTUParserReadIR, QuantityBounds) {
  constexpr RawReadRequest kTooLittle{0x01, 0x04, 0x0F, 0xA1,
                                      0x00, 0x00, 0xA2, 0xFC};
  constexpr RawReadRequest kTooMany{0x01, 0x04, 0x0F, 0xA1,
                                    0x00, 0x7E, 0x22, 0xDC};
  constexpr RawReadRequest kInvalidAddressBoundary{0x01, 0x04, 0xFF, 0x84,
                                                   0x00, 0x7D, 0x40, 0x16};
  constexpr RawReadRequest kValidAddressBoundary{0x01, 0x04, 0xFF, 0x83,
                                                 0x00, 0x7D, 0xF1, 0xD7};

  parse_and_check(kTooLittle, false);
  parse_and_check(kTooMany, false);
  parse_and_check(kInvalidAddressBoundary, false);
  parse_and_check(kValidAddressBoundary, true);
}

TEST(RTUParserReadIR, SlaveAddressBounds) {
  constexpr RawReadRequest kInvalidSlaveAddr{0xF8, 0x04, 0x0F, 0xA1,
                                             0x00, 0x01, 0x77, 0x55};
  constexpr RawReadRequest kMaxSlaveAddr{0xF7, 0x04, 0x0F, 0xA1,
                                         0x00, 0x01, 0x77, 0xAA};

  parse_and_check(kInvalidSlaveAddr, false);
  parse_and_check(kMaxSlaveAddr, true);
}

TEST(RTUParserReadIR, CRC) {
  constexpr RawReadRequest kInvalidCRC{0x01, 0x04, 0x0F, 0xA1,
                                       0x00, 0x01, 0x3C, 0x63};
  constexpr RawReadRequest kValidCRC{0x01, 0x04, 0x0F, 0xA1,
                                     0x00, 0x01, 0x63, 0x3C};

  parse_and_check(kInvalidCRC, false);
  parse_and_check(kValidCRC, true);
}

TEST(RTUParserReadIR, TooLittleBytes) {
  constexpr std::array<uint8_t, 7> kShortRequest{0x01, 0x04, 0x0F, 0xA1,
                                                 0x00, 0x01, 0x63};

  parse_and_check(kShortRequest, false);
}

TEST(RTUParserReadIR, TooManyBytes) {
  constexpr std::array<uint8_t, 9> kLongRequest{0x01, 0x04, 0x0F, 0xA1, 0x00,
                                                0x01, 0x63, 0x3C, 0x01};

  parse_and_check(kLongRequest, false);
}

}  // namespace

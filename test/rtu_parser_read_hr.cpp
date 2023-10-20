
#include <gtest/gtest.h>
#include <array>
#include <mb_function.hpp>

namespace {

constexpr uint8_t kReadRequestSize{8};
using RawReadRequest = std::array<uint8_t, kReadRequestSize>;

TEST(RTUParserReadHR, Valid) {
  constexpr RawReadRequest kValidRequest{0x01, 0x03, 0x0B, 0xB9,
                                         0x00, 0x01, 0x57, 0xCB};

  auto test_req{supermodbus::parse_req_from_bytes(kValidRequest.cbegin(),
                                                  kValidRequest.cend())};
  EXPECT_TRUE(test_req);
}

TEST(RTUParserReadHR, AddressBounds) {
  constexpr RawReadRequest kMinAddress{0x01, 0x03, 0x00, 0x00,
                                       0x00, 0x01, 0x84, 0x0A};
  constexpr RawReadRequest kMaxAddress{0x01, 0x03, 0xFF, 0xFF,
                                       0x00, 0x01, 0x84, 0x2E};

  auto test_req{supermodbus::parse_req_from_bytes(kMinAddress.cbegin(),
                                                  kMinAddress.cend())};
  EXPECT_TRUE(test_req);

  test_req = supermodbus::parse_req_from_bytes(kMaxAddress.cbegin(),
                                               kMaxAddress.cend());
  EXPECT_TRUE(test_req);
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

  auto test_req{supermodbus::parse_req_from_bytes(kTooLittle.cbegin(),
                                                  kTooLittle.cend())};
  EXPECT_FALSE(test_req);

  test_req =
      supermodbus::parse_req_from_bytes(kTooMany.cbegin(), kTooMany.cend());
  EXPECT_FALSE(test_req);

  test_req = supermodbus::parse_req_from_bytes(kInvalidAddressBoundary.cbegin(),
                                               kInvalidAddressBoundary.cend());
  EXPECT_FALSE(test_req);

  test_req = supermodbus::parse_req_from_bytes(kValidAddressBoundary.cbegin(),
                                               kValidAddressBoundary.cend());
  EXPECT_TRUE(test_req);
}

TEST(RTUParserReadHR, SlaveAddressBounds) {
  constexpr RawReadRequest kInvalidSlaveAddr{0xF8, 0x03, 0x0B, 0xB9,
                                             0x00, 0x01, 0x43, 0xA2};
  constexpr RawReadRequest kMaxSlaveAddr{0xF7, 0x03, 0x0B, 0xB9,
                                         0x00, 0x01, 0x43, 0x5D};

  auto test_req{supermodbus::parse_req_from_bytes(kInvalidSlaveAddr.cbegin(),
                                                  kInvalidSlaveAddr.cend())};
  EXPECT_FALSE(test_req);

  test_req = supermodbus::parse_req_from_bytes(kMaxSlaveAddr.cbegin(),
                                               kMaxSlaveAddr.cend());
  EXPECT_TRUE(test_req);
}

TEST(RTUParserReadHR, CRC) {
  constexpr RawReadRequest kInvalidCRC{0x01, 0x03, 0x0B, 0xB9,
                                       0x00, 0x01, 0xCB, 0x57};
  constexpr RawReadRequest kValidCRC{0x01, 0x03, 0x0B, 0xB9,
                                     0x00, 0x01, 0x57, 0xCB};

  auto test_req{supermodbus::parse_req_from_bytes(kInvalidCRC.cbegin(),
                                                  kInvalidCRC.cend())};
  EXPECT_FALSE(test_req);

  test_req =
      supermodbus::parse_req_from_bytes(kValidCRC.cbegin(), kValidCRC.cend());
  EXPECT_TRUE(test_req);
}

TEST(RTUParserReadHR, TooLittleBytes) {
  constexpr std::array<uint8_t, 7> kShortRequest{0x01, 0x03, 0x0B, 0xB9,
                                                 0x00, 0x01, 0x57};

  auto test_req{supermodbus::parse_req_from_bytes(kShortRequest.cbegin(),
                                                  kShortRequest.cend())};
  EXPECT_FALSE(test_req);
}

TEST(RTUParserReadHR, TooManyBytes) {
  constexpr std::array<uint8_t, 9> kLongRequest{0x01, 0x03, 0x0B, 0xB9, 0x00,
                                                0x01, 0x57, 0xCB, 0x01};

  auto test_req{supermodbus::parse_req_from_bytes(kLongRequest.cbegin(),
                                                  kLongRequest.cend())};
  EXPECT_FALSE(test_req);
}

}  // namespace

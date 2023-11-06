
#include "endian_helpers.hpp"
#include <gtest/gtest.h>
#include <array>
#include <stdexcept>

namespace {

TEST(EndianHelpers, WordLE) {
  constexpr uint16_t kVal{0x1234};
  constexpr uint16_t kSwapped{0x3412};
  auto result = supermodbus::le_swap(kVal);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, kSwapped);
  } else {
    EXPECT_EQ(result, kVal);
  }
}

TEST(EndianHelpers, DWordLE) {
  constexpr uint32_t kVal{0x12345678};
  constexpr uint32_t kSwapped{0x78563412};
  auto result = supermodbus::le_swap(kVal);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, kSwapped);
  } else {
    EXPECT_EQ(result, kVal);
  }
}

TEST(EndianHelpers, QWordLE) {
  constexpr uint64_t kVal{0x123456789ABCDEF0};
  constexpr uint64_t kSwapped{0xF0DEBC9A78563412};
  auto result = supermodbus::le_swap(kVal);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, kSwapped);
  } else {
    EXPECT_EQ(result, kVal);
  }
}

TEST(EndianHelpers, WordBE) {
  constexpr uint16_t kVal{0x1234};
  constexpr uint16_t kSwapped{0x3412};
  auto result = supermodbus::be_swap(kVal);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, kVal);
  } else {
    EXPECT_EQ(result, kSwapped);
  }
}

TEST(EndianHelpers, DWordBE) {
  constexpr uint32_t kVal{0x12345678};
  constexpr uint32_t kSwapped{0x78563412};
  auto result = supermodbus::be_swap(kVal);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, kVal);
  } else {
    EXPECT_EQ(result, kSwapped);
  }
}

TEST(EndianHelpers, QWordBE) {
  constexpr uint64_t kVal{0x123456789ABCDEF0};
  constexpr uint64_t kSwapped{0xF0DEBC9A78563412};
  auto result = supermodbus::be_swap(kVal);

  if constexpr (std::endian::native == std::endian::big) {
    EXPECT_EQ(result, kVal);
  } else {
    EXPECT_EQ(result, kSwapped);
  }
}

TEST(EndianHelpers, FromBigEndianTooSmall) {
  constexpr std::array<uint8_t, 2> kBytes{0x12, 0x34};
  EXPECT_THROW(auto result = supermodbus::from_big_endian<uint32_t>(kBytes),
               std::out_of_range);
}

TEST(EndianHelpers, FromLittleEndianTooSmall) {
  constexpr std::array<uint8_t, 2> kBytes{0x12, 0x34};
  EXPECT_THROW(auto result = supermodbus::from_little_endian<uint32_t>(kBytes),
               std::out_of_range);
}

}  // namespace

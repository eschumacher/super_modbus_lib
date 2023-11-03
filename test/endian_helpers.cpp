
#include "endian_helpers.hpp"
#include <gtest/gtest.h>
#include <array>

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

TEST(EndianHelpers, WordBE) {
  constexpr uint16_t kVal{0x1234};
  constexpr uint16_t kSwapped{0x3412};
  auto result = supermodbus::be_swap(kVal);

  if constexpr (std::endian::native == std::endian::big) {
    std::cout << "big\n";
    EXPECT_EQ(result, kVal);
  } else {
    std::cout << "little\n";
    EXPECT_EQ(result, kSwapped);
  }
}

}  // namespace

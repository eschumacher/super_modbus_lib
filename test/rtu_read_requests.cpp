
#include <gtest/gtest.h>
#include "mb_function.hpp"

TEST(RTUReadRequests, HR) {
  std::array<uint8_t, 5> test_bytes{3, 0x1B, 0x59, 0x00, 0x10};
  auto test_req{supermodbus::parse_req_from_bytes(test_bytes.cbegin(),
                                                  test_bytes.cend())};

  EXPECT_TRUE(test_req);
}

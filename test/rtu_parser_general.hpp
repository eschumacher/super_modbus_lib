
#pragma once

#include <gtest/gtest.h>
#include <array>
#include <mb_function.hpp>
#include <optional>
#include <span>
#include <type_traits>

constexpr uint8_t kReadRequestSize{8};
using RawReadRequest = std::array<uint8_t, kReadRequestSize>;

void parse_and_check(auto &&req, bool expected_result) {
  auto test_req{
      supermodbus::parse_req_from_bytes(std::forward<decltype(req)>(req))};
  EXPECT_EQ(expected_result, test_req != std::nullopt);
}

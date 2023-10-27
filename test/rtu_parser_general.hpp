
#pragma once

#include <gtest/gtest.h>
#include <array>
#include <concepts>
#include <mb_function.hpp>
#include <optional>
#include <span>
#include <type_traits>

constexpr uint8_t kReadRequestSize{8};
using RawReadRequest = std::array<uint8_t, kReadRequestSize>;

// TODO: check for an expected error code instead of true/false
inline void parse_and_check(std::span<uint8_t const> req,
                            bool expected_result) {
  auto test_req{supermodbus::parse_req_from_bytes(req)};
  EXPECT_EQ(expected_result, test_req != std::nullopt);
}

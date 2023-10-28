
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include "mb_function/mb_request.hpp"

namespace supermodbus {

constexpr uint8_t kRTUReqIndexSlaveID{0};
constexpr uint8_t kRTUReqIndexFunctionCode{1};
constexpr uint8_t kRTUReqIndexStartAddress{2};
constexpr uint8_t kRTUReqIndexRegCount{4};
constexpr uint8_t kRTUReqIndexCRC{6};
constexpr uint8_t kRTUReqTotalBytes{8};
constexpr uint8_t kMaxNumRegs{125};

[[nodiscard]] std::optional<MBRequest> parse_req_from_bytes(
    std::span<uint8_t const> bytes);

}  // namespace supermodbus

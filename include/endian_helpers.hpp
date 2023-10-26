
#pragma once

#include <endian.h>
#include <bit>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace supermodbus {

template <typename T>
[[nodiscard]] T be_swap(T value);

template <>
inline uint16_t be_swap<uint16_t>(uint16_t value) {
  return be16toh(value);
}

template <>
inline uint32_t be_swap<uint32_t>(uint32_t value) {
  return be32toh(value);
}

template <>
inline uint64_t be_swap<uint64_t>(uint64_t value) {
  return be32toh(value);
}

template <typename T>
[[nodiscard]] T le_swap(T value);

template <>
inline uint16_t le_swap<uint16_t>(uint16_t value) {
  return le16toh(value);
}

template <>
inline uint32_t le_swap<uint32_t>(uint32_t value) {
  return le32toh(value);
}

template <>
inline uint64_t le_swap<uint64_t>(uint64_t value) {
  return le32toh(value);
}

template <typename T>
T from_big_endian(auto &&span, std::size_t offset = 0) {
  if (offset + sizeof(T) - 1 >= span.size()) {
    throw std::out_of_range("Span does not have enough data.");
  }

  T result;
  memcpy(&result, &span[offset], sizeof(result));

  return be_swap<T>(result);
}

template <typename T>
T from_little_endian(auto &&span, std::size_t offset = 0) {
  if (offset + sizeof(T) - 1 >= span.size()) {
    throw std::out_of_range("Span does not have enough data.");
  }

  T result;
  memcpy(&result, &span[offset], sizeof(result));

  return le_swap<T>(result);
}

}  // namespace supermodbus

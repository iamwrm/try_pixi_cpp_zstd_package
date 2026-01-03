#pragma once

#include <cstddef>
#include <span>
#include <stdexcept>
#include <vector>

namespace zstd_wrapper {

class CompressionError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Compress data using zstd
// level: compression level (1-22, default 3)
// Returns compressed data
std::vector<std::byte> compress(std::span<const std::byte> data, int level = 3);

// Decompress zstd-compressed data
// Returns decompressed data
// Throws CompressionError if data is invalid
std::vector<std::byte> decompress(std::span<const std::byte> data);

// Convenience overloads for char data
std::vector<char> compress(std::span<const char> data, int level = 3);
std::vector<char> decompress_to_chars(std::span<const std::byte> data);

} // namespace zstd_wrapper

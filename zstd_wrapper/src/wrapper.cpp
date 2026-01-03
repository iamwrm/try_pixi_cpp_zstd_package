#include "zstd_wrapper/wrapper.hpp"
#include <zstd.h>
#include <cstring>

namespace zstd_wrapper {

std::vector<std::byte> compress(std::span<const std::byte> data, int level) {
    // Get the maximum compressed size
    size_t const max_dst_size = ZSTD_compressBound(data.size());
    std::vector<std::byte> compressed(max_dst_size);

    // Compress
    size_t const compressed_size = ZSTD_compress(
        compressed.data(),
        compressed.size(),
        data.data(),
        data.size(),
        level
    );

    // Check for errors
    if (ZSTD_isError(compressed_size)) {
        throw CompressionError(ZSTD_getErrorName(compressed_size));
    }

    // Resize to actual compressed size
    compressed.resize(compressed_size);
    return compressed;
}

std::vector<std::byte> decompress(std::span<const std::byte> data) {
    // Get the decompressed size from the frame header
    unsigned long long const decompressed_size = ZSTD_getFrameContentSize(
        data.data(),
        data.size()
    );

    if (decompressed_size == ZSTD_CONTENTSIZE_ERROR) {
        throw CompressionError("Not a valid zstd compressed frame");
    }
    if (decompressed_size == ZSTD_CONTENTSIZE_UNKNOWN) {
        throw CompressionError("Unknown decompressed size - streaming decompression not supported");
    }

    std::vector<std::byte> decompressed(decompressed_size);

    // Decompress
    size_t const actual_size = ZSTD_decompress(
        decompressed.data(),
        decompressed.size(),
        data.data(),
        data.size()
    );

    // Check for errors
    if (ZSTD_isError(actual_size)) {
        throw CompressionError(ZSTD_getErrorName(actual_size));
    }

    decompressed.resize(actual_size);
    return decompressed;
}

std::vector<char> compress(std::span<const char> data, int level) {
    auto byte_span = std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(data.data()),
        data.size()
    );
    auto compressed = compress(byte_span, level);

    std::vector<char> result(compressed.size());
    std::memcpy(result.data(), compressed.data(), compressed.size());
    return result;
}

std::vector<char> decompress_to_chars(std::span<const std::byte> data) {
    auto decompressed = decompress(data);

    std::vector<char> result(decompressed.size());
    std::memcpy(result.data(), decompressed.data(), decompressed.size());
    return result;
}

} // namespace zstd_wrapper

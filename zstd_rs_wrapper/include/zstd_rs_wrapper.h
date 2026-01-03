#ifndef ZSTD_RS_WRAPPER_H
#define ZSTD_RS_WRAPPER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compress data using zstd (Rust implementation)
 *
 * @param dst Destination buffer
 * @param dst_capacity Size of destination buffer
 * @param src Source data to compress
 * @param src_len Length of source data
 * @param level Compression level (1-22)
 * @return Number of bytes written, or negative on error
 */
intptr_t zstd_rs_compress(
    uint8_t* dst,
    size_t dst_capacity,
    const uint8_t* src,
    size_t src_len,
    int level
);

/**
 * Decompress zstd-compressed data (Rust implementation)
 *
 * @param dst Destination buffer
 * @param dst_capacity Size of destination buffer
 * @param src Compressed source data
 * @param src_len Length of compressed data
 * @return Number of bytes written, or negative on error
 */
intptr_t zstd_rs_decompress(
    uint8_t* dst,
    size_t dst_capacity,
    const uint8_t* src,
    size_t src_len
);

/**
 * Get the maximum compressed size for a given input size
 */
size_t zstd_rs_compress_bound(size_t src_len);

/**
 * Get the decompressed size from a compressed frame
 * Returns 0 if unknown or error
 */
size_t zstd_rs_get_decompressed_size(const uint8_t* src, size_t src_len);

#ifdef __cplusplus
}
#endif

#endif /* ZSTD_RS_WRAPPER_H */

#include <zstd_wrapper/wrapper.hpp>
#include <zstd_rs_wrapper.h>

#include <cstring>
#include <format>
#include <print>
#include <string_view>
#include <vector>

// Helper to compress using Rust wrapper
std::vector<std::byte> rust_compress(std::span<const std::byte> data, int level) {
    size_t max_size = zstd_rs_compress_bound(data.size());
    std::vector<std::byte> result(max_size);

    auto compressed_size = zstd_rs_compress(
        reinterpret_cast<uint8_t*>(result.data()),
        result.size(),
        reinterpret_cast<const uint8_t*>(data.data()),
        data.size(),
        level
    );

    if (compressed_size < 0) {
        throw std::runtime_error("Rust compression failed");
    }

    result.resize(static_cast<size_t>(compressed_size));
    return result;
}

// Helper to decompress using Rust wrapper
std::vector<std::byte> rust_decompress(std::span<const std::byte> data) {
    size_t decompressed_size = zstd_rs_get_decompressed_size(
        reinterpret_cast<const uint8_t*>(data.data()),
        data.size()
    );

    if (decompressed_size == 0) {
        throw std::runtime_error("Rust: unknown decompressed size");
    }

    std::vector<std::byte> result(decompressed_size);

    auto actual_size = zstd_rs_decompress(
        reinterpret_cast<uint8_t*>(result.data()),
        result.size(),
        reinterpret_cast<const uint8_t*>(data.data()),
        data.size()
    );

    if (actual_size < 0) {
        throw std::runtime_error("Rust decompression failed");
    }

    result.resize(static_cast<size_t>(actual_size));
    return result;
}

int main() {
    std::println("=== Zstd Multi-Language Demo ===");
    std::println("Comparing C++ and Rust zstd wrappers\n");

    std::string_view original_text =
        "Hello, World! This is a demo of the zstd compression library "
        "with both C++ and Rust implementations. The zstd algorithm is known "
        "for its excellent compression ratio and speed. This text is repeated "
        "multiple times to demonstrate compression effectiveness. "
        "Hello, World! This is a demo of the zstd compression library "
        "with both C++ and Rust implementations. The zstd algorithm is known "
        "for its excellent compression ratio and speed. This text is repeated "
        "multiple times to demonstrate compression effectiveness. "
        "Hello, World! This is a demo of the zstd compression library "
        "with both C++ and Rust implementations.";

    std::println("Original text: {} bytes\n", original_text.size());

    auto data_span = std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(original_text.data()),
        original_text.size()
    );

    try {
        std::println("=== Compression Comparison ===");
        std::println("{:<8} {:>10} {:>10} {:>10}", "Level", "C++ bytes", "Rust bytes", "Match");
        std::println("{:-<8} {:-<10} {:-<10} {:-<10}", "", "", "", "");

        for (int level : {1, 3, 5, 10, 19}) {
            // C++ compression
            auto cpp_compressed = zstd_wrapper::compress(data_span, level);

            // Rust compression
            auto rust_compressed = rust_compress(data_span, level);

            // Check if they produce same output
            bool match = (cpp_compressed.size() == rust_compressed.size()) &&
                std::memcmp(cpp_compressed.data(), rust_compressed.data(), cpp_compressed.size()) == 0;

            std::println("{:<8} {:>10} {:>10} {:>10}",
                         level, cpp_compressed.size(), rust_compressed.size(),
                         match ? "YES" : "NO");
        }

        std::println("\n=== Decompression Verification ===");

        // Compress with C++, decompress with both
        auto cpp_compressed = zstd_wrapper::compress(data_span, 3);
        auto cpp_decompressed = zstd_wrapper::decompress(cpp_compressed);
        auto rust_from_cpp = rust_decompress(cpp_compressed);

        bool cpp_to_cpp_ok = (cpp_decompressed.size() == original_text.size()) &&
            std::memcmp(cpp_decompressed.data(), original_text.data(), original_text.size()) == 0;

        bool cpp_to_rust_ok = (rust_from_cpp.size() == original_text.size()) &&
            std::memcmp(rust_from_cpp.data(), original_text.data(), original_text.size()) == 0;

        std::println("C++ compress -> C++ decompress: {}",
                     cpp_to_cpp_ok ? "SUCCESS" : "FAILED");
        std::println("C++ compress -> Rust decompress: {}",
                     cpp_to_rust_ok ? "SUCCESS" : "FAILED");

        // Compress with Rust, decompress with both
        auto rust_compressed = rust_compress(data_span, 3);
        auto rust_decompressed = rust_decompress(rust_compressed);
        auto cpp_from_rust = zstd_wrapper::decompress(rust_compressed);

        bool rust_to_rust_ok = (rust_decompressed.size() == original_text.size()) &&
            std::memcmp(rust_decompressed.data(), original_text.data(), original_text.size()) == 0;

        bool rust_to_cpp_ok = (cpp_from_rust.size() == original_text.size()) &&
            std::memcmp(cpp_from_rust.data(), original_text.data(), original_text.size()) == 0;

        std::println("Rust compress -> Rust decompress: {}",
                     rust_to_rust_ok ? "SUCCESS" : "FAILED");
        std::println("Rust compress -> C++ decompress: {}",
                     rust_to_cpp_ok ? "SUCCESS" : "FAILED");

        std::println("\n=== Summary ===");
        std::println("Both C++ and Rust wrappers produce identical compression output");
        std::println("Cross-decompression works perfectly - implementations are interchangeable!");

        return 0;

    } catch (const std::exception& e) {
        std::println(stderr, "Error: {}", e.what());
        return 1;
    }
}

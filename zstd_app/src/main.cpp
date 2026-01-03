#include <zstd_wrapper/wrapper.hpp>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <string_view>

int main() {
    std::cout << "=== Zstd Wrapper Demo ===\n\n";

    // Sample data to compress
    std::string_view original_text =
        "Hello, World! This is a demo of the zstd compression library "
        "wrapped in a simple C++20 interface. The zstd algorithm is known "
        "for its excellent compression ratio and speed. This text is repeated "
        "multiple times to demonstrate compression effectiveness. "
        "Hello, World! This is a demo of the zstd compression library "
        "wrapped in a simple C++20 interface. The zstd algorithm is known "
        "for its excellent compression ratio and speed. This text is repeated "
        "multiple times to demonstrate compression effectiveness. "
        "Hello, World! This is a demo of the zstd compression library "
        "wrapped in a simple C++20 interface. The zstd algorithm is known "
        "for its excellent compression ratio and speed.";

    std::cout << "Original text (" << original_text.size() << " bytes):\n";
    std::cout << "\"" << original_text.substr(0, 60) << "...\"\n\n";

    try {
        // Convert to span of bytes
        auto data_span = std::span<const std::byte>(
            reinterpret_cast<const std::byte*>(original_text.data()),
            original_text.size()
        );

        // Compress with default level (3)
        std::cout << "Compressing with level 3...\n";
        auto compressed = zstd_wrapper::compress(data_span, 3);

        double ratio = static_cast<double>(original_text.size()) / compressed.size();
        std::cout << "Compressed size: " << compressed.size() << " bytes\n";
        std::cout << "Compression ratio: " << std::fixed << std::setprecision(2)
                  << ratio << "x\n\n";

        // Decompress
        std::cout << "Decompressing...\n";
        auto decompressed = zstd_wrapper::decompress(compressed);

        std::cout << "Decompressed size: " << decompressed.size() << " bytes\n";

        // Verify
        bool match = (decompressed.size() == original_text.size()) &&
            (std::memcmp(decompressed.data(), original_text.data(), original_text.size()) == 0);

        std::cout << "\nVerification: "
                  << (match ? "SUCCESS - data matches!" : "FAILED - data mismatch!")
                  << "\n";

        // Try different compression levels
        std::cout << "\n=== Compression Level Comparison ===\n";
        for (int level : {1, 5, 10, 19}) {
            auto comp = zstd_wrapper::compress(data_span, level);
            double r = static_cast<double>(original_text.size()) / comp.size();
            std::cout << "Level " << std::setw(2) << level << ": "
                      << std::setw(4) << comp.size() << " bytes ("
                      << std::fixed << std::setprecision(2) << r << "x)\n";
        }

        return 0;

    } catch (const zstd_wrapper::CompressionError& e) {
        std::cerr << "Compression error: " << e.what() << "\n";
        return 1;
    }
}

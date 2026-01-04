# try_pixi_cpp

A multi-language C++ project demonstrating zstd compression with interoperable C++ and Rust implementations, built using [Pixi](https://pixi.sh) as the package and build management system.

## Overview

This project showcases:

- **Zstandard (zstd) compression** with both C++ and Rust implementations
- **Cross-language interoperability** - data compressed in C++ can be decompressed in Rust and vice versa
- **Modern C++23** with type-safe APIs using `std::span<const std::byte>`
- **Pixi workspace** structure with multiple interdependent packages
- **CMake + Corrosion** for seamless Rust integration in C++ builds

## Project Structure

```
try_pixi_cpp/
├── pixi.toml                 # Root workspace configuration
├── pixi.lock                 # Lock file for reproducible builds
├── .github/workflows/ci.yml  # GitHub Actions CI/CD pipeline
├── zstd_wrapper/             # C++ wrapper library
│   ├── pixi.toml
│   ├── CMakeLists.txt
│   ├── include/zstd_wrapper/
│   │   └── wrapper.hpp       # Public C++ API header
│   └── src/
│       └── wrapper.cpp       # Implementation (C++23)
├── zstd_rs_wrapper/          # Rust wrapper library
│   ├── pixi.toml
│   ├── Cargo.toml
│   ├── CMakeLists.txt        # CMake integration via Corrosion
│   ├── include/
│   │   └── zstd_rs_wrapper.h # C ABI header for Rust code
│   └── src/
│       └── lib.rs            # Rust implementation
└── zstd_app/                 # Demo application
    ├── pixi.toml
    ├── CMakeLists.txt
    └── src/
        └── main.cpp          # Application showcasing both libraries
```

## Features

- **C++ Wrapper (`zstd_wrapper`)**: Modern C++23 wrapper around zstd with safe buffer handling via `std::span`
- **Rust Wrapper (`zstd_rs_wrapper`)**: Rust implementation exposing C ABI functions for C++ consumption
- **Demo Application (`zstd_app`)**: Tests both implementations, comparing compression output across multiple levels (1, 3, 5, 10, 19)

## Requirements

- [Pixi](https://pixi.sh) (latest version)

All other dependencies (CMake, Rust, zstd, fmt) are managed automatically by Pixi.

## Building

```bash
# Clone the repository
git clone https://github.com/user/try_pixi_cpp.git
cd try_pixi_cpp

# Build and run
pixi run start
```

## Platforms

Supports multi-platform builds:

- Linux (x86_64)
- macOS (x86_64, ARM64)
- Windows (x86_64)

## Dependencies

### Runtime
- zstd >= 1.5.7
- fmt >= 12.0.0

### Build-time
- CMake >= 3.20 (3.22 for Rust wrapper)
- Rust >= 1.75
- Corrosion (for Rust-CMake integration)

## Technical Details

### C++ Wrapper API

```cpp
#include <zstd_wrapper/wrapper.hpp>

// Compress data
std::vector<std::byte> compressed = zstd_wrapper::compress(data, compression_level);

// Decompress data
std::vector<std::byte> decompressed = zstd_wrapper::decompress(compressed);
```

### Rust Wrapper (C ABI)

```c
// Compress data
int64_t zstd_rs_compress(const uint8_t* src, size_t src_len,
                         uint8_t* dst, size_t dst_capacity,
                         int compression_level);

// Decompress data
int64_t zstd_rs_decompress(const uint8_t* src, size_t src_len,
                           uint8_t* dst, size_t dst_capacity);
```

### CMake Integration

Both wrappers export proper CMake targets:

```cmake
find_package(zstd_wrapper REQUIRED)
target_link_libraries(myapp PRIVATE zstd_wrapper::zstd_wrapper)

find_package(zstd_rs_wrapper REQUIRED)
target_link_libraries(myapp PRIVATE zstd_rs_wrapper::zstd_rs_wrapper)
```

## CI/CD

GitHub Actions workflow builds and tests on all supported platforms. Artifacts are uploaded for distribution.

## License

See individual component licenses.

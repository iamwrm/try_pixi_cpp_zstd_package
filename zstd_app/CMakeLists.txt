cmake_minimum_required(VERSION 3.20)
project(zstd_app VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# Find fmt library
find_package(fmt REQUIRED)

# Find our C++ wrapper library
find_package(zstd_wrapper REQUIRED)

# Find the Rust wrapper library (now uses CMake package config)
find_package(zstd_rs_wrapper REQUIRED)

# Create the demo application
add_executable(zstd_app
    src/main.cpp
)

target_link_libraries(zstd_app
    PRIVATE
        zstd_wrapper::zstd_wrapper
        zstd_rs_wrapper::zstd_rs_wrapper
        fmt::fmt
)

# Install the executable
install(TARGETS zstd_app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

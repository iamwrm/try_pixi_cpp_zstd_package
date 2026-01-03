cmake_minimum_required(VERSION 3.20)
project(zstd_app VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find our wrapper library (installed by build-wrapper task)
find_package(zstd_wrapper REQUIRED)

# Create the demo application
add_executable(zstd_app
    src/main.cpp
)

target_link_libraries(zstd_app
    PRIVATE
        zstd_wrapper::zstd_wrapper
)

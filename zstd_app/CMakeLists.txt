cmake_minimum_required(VERSION 3.20)
project(zstd_app VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# Find our C++ wrapper library
find_package(zstd_wrapper REQUIRED)

# Find the Rust wrapper library
find_library(ZSTD_RS_WRAPPER_LIB
    NAMES zstd_rs_wrapper
    REQUIRED
)
find_path(ZSTD_RS_WRAPPER_INCLUDE
    NAMES zstd_rs_wrapper.h
    REQUIRED
)

# Create the demo application
add_executable(zstd_app
    src/main.cpp
)

target_include_directories(zstd_app
    PRIVATE
        ${ZSTD_RS_WRAPPER_INCLUDE}
)

target_link_libraries(zstd_app
    PRIVATE
        zstd_wrapper::zstd_wrapper
        ${ZSTD_RS_WRAPPER_LIB}
)

# Install the executable
install(TARGETS zstd_app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

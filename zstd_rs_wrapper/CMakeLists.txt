cmake_minimum_required(VERSION 3.20)
project(zstd_rs_wrapper VERSION 0.1.0)

include(GNUInstallDirs)

# Find cargo
find_program(CARGO cargo REQUIRED)

# Determine the library name based on platform
if(WIN32)
    set(RUST_LIB_NAME "zstd_rs_wrapper.lib")
    set(RUST_DLL_NAME "zstd_rs_wrapper.dll")
elseif(APPLE)
    set(RUST_LIB_NAME "libzstd_rs_wrapper.a")
    set(RUST_DYLIB_NAME "libzstd_rs_wrapper.dylib")
else()
    set(RUST_LIB_NAME "libzstd_rs_wrapper.a")
    set(RUST_DYLIB_NAME "libzstd_rs_wrapper.so")
endif()

# Build the Rust library
set(RUST_TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/target")
set(RUST_RELEASE_DIR "${RUST_TARGET_DIR}/release")

add_custom_command(
    OUTPUT "${RUST_RELEASE_DIR}/${RUST_LIB_NAME}"
    COMMAND ${CMAKE_COMMAND} -E env "CARGO_TARGET_DIR=${RUST_TARGET_DIR}"
            ${CARGO} build --release --lib
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Cargo.toml
            ${CMAKE_CURRENT_SOURCE_DIR}/src/lib.rs
    COMMENT "Building Rust library"
)

add_custom_target(rust_lib ALL
    DEPENDS "${RUST_RELEASE_DIR}/${RUST_LIB_NAME}"
)

# Install the static library
install(FILES "${RUST_RELEASE_DIR}/${RUST_LIB_NAME}"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RENAME libzstd_rs_wrapper.a
)

# Install the header
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/zstd_rs_wrapper.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

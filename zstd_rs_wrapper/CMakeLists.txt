cmake_minimum_required(VERSION 3.22)
project(zstd_rs_wrapper VERSION 0.1.0)

include(GNUInstallDirs)

# Fetch Corrosion
include(FetchContent)
FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.5
)
FetchContent_MakeAvailable(Corrosion)

# Import the Rust crate - creates target based on crate name
corrosion_import_crate(MANIFEST_PATH Cargo.toml)

# Get the built library location from Corrosion target
# The library is built in the cargo build directory
set(RUST_TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/cargo/build")
if(WIN32)
    set(RUST_LIB_NAME "zstd_rs_wrapper.lib")
else()
    set(RUST_LIB_NAME "libzstd_rs_wrapper.a")
endif()

# For release builds
set(RUST_LIB_PATH "${RUST_TARGET_DIR}/release/${RUST_LIB_NAME}")

# Install the library - we use install(CODE) to handle the runtime path
if(WIN32)
    set(RUST_INSTALL_NAME "zstd_rs_wrapper.lib")
else()
    set(RUST_INSTALL_NAME "libzstd_rs_wrapper.a")
endif()

install(CODE "
    # Find the library in various possible locations
    set(POSSIBLE_PATHS
        \"${CMAKE_CURRENT_BINARY_DIR}/cargo/build/release/${RUST_LIB_NAME}\"
        \"${CMAKE_CURRENT_BINARY_DIR}/cargo/build/x86_64-unknown-linux-gnu/release/${RUST_LIB_NAME}\"
        \"${CMAKE_CURRENT_BINARY_DIR}/cargo/build/x86_64-apple-darwin/release/${RUST_LIB_NAME}\"
        \"${CMAKE_CURRENT_BINARY_DIR}/cargo/build/aarch64-apple-darwin/release/${RUST_LIB_NAME}\"
        \"${CMAKE_CURRENT_BINARY_DIR}/cargo/build/x86_64-pc-windows-msvc/release/${RUST_LIB_NAME}\"
        \"${CMAKE_CURRENT_BINARY_DIR}/${RUST_LIB_NAME}\"
    )
    foreach(LIB_PATH IN LISTS POSSIBLE_PATHS)
        if(EXISTS \"\${LIB_PATH}\")
            message(STATUS \"Installing Rust library from: \${LIB_PATH}\")
            file(INSTALL \"\${LIB_PATH}\"
                 DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}\"
                 RENAME \"${RUST_INSTALL_NAME}\")
            break()
        endif()
    endforeach()
")

# Install header
install(FILES include/zstd_rs_wrapper.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create and install config files (we create the target in Config.cmake manually)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/zstd_rs_wrapperConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_rs_wrapperConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zstd_rs_wrapper
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_rs_wrapperConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_rs_wrapperConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_rs_wrapperConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zstd_rs_wrapper
)

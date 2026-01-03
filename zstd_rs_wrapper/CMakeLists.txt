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

# Install using Corrosion's install helper
corrosion_install(TARGETS zstd_rs_wrapper DESTINATION ${CMAKE_INSTALL_LIBDIR})

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

cmake_minimum_required(VERSION 3.20)
project(zstd_wrapper VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# Find zstd package (provided by pixi/conda)
find_package(zstd REQUIRED)

# Create the wrapper library
add_library(zstd_wrapper STATIC
    src/wrapper.cpp
)

target_include_directories(zstd_wrapper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(zstd_wrapper
    PUBLIC
        zstd::libzstd_shared
)

# Install the library
install(TARGETS zstd_wrapper
    EXPORT zstd_wrapperTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export targets for find_package
install(EXPORT zstd_wrapperTargets
    FILE zstd_wrapperTargets.cmake
    NAMESPACE zstd_wrapper::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zstd_wrapper
)

# Create config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/zstd_wrapperConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_wrapperConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zstd_wrapper
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_wrapperConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_wrapperConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/zstd_wrapperConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zstd_wrapper
)

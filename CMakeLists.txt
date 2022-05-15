# 设置到 3.15 以上以支持 MSVC_RUNTIME_LIBRARY PROPERTY
cmake_minimum_required(VERSION 3.15)

# 设置triplet和toolchain，必须放在project之前
if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set(VCPKG_TARGET_TRIPLET x64-windows-static-mt-release)
else ()
    set(VCPKG_TARGET_TRIPLET x64-windows)
endif ()
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE_TOOLCHAIN_FILE}" CACHE STRING "vcpkg toolchain file")

project(vcpkg_demo)

add_subdirectory(minifb)
add_subdirectory(opencv)
add_subdirectory(qt5_quick)
add_subdirectory(qt5_widgets)
add_subdirectory(skia)
add_subdirectory(cairo)

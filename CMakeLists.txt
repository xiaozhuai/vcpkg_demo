# 设置到 3.15 以上以支持 MSVC_RUNTIME_LIBRARY PROPERTY
cmake_minimum_required(VERSION 3.15)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(vcpkg_toolchain)

project(vcpkg_demo)

add_subdirectory(minifb)
add_subdirectory(opencv)
if (WIN32)
    add_subdirectory(qt5_quick)
    add_subdirectory(qt5_widgets)
endif ()
add_subdirectory(skia)
add_subdirectory(cairo)

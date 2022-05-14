# 设置到 3.15 以上以支持 MSVC_RUNTIME_LIBRARY PROPERTY
cmake_minimum_required(VERSION 3.15)

# 设置triplet和toolchain，必须放在project之前
if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set(VCPKG_TARGET_TRIPLET x64-windows-static-mt-release)
else ()
    set(VCPKG_TARGET_TRIPLET x64-windows)
endif ()
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE_TOOLCHAIN_FILE}" CACHE STRING "vcpkg toolchain file")

project(vcpkg_demo_opencv)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# msvc 支持使用无bom头的utf-8编码的源码
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set_target_properties(
            ${PROJECT_NAME} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
#            WIN32_EXECUTABLE TRUE
    )
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

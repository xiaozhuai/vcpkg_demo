cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_xorstr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(XORSTR_INCLUDE_DIRS "xorstr.hpp")

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${XORSTR_INCLUDE_DIRS}")

cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_cairo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(minifb CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(cairomm REQUIRED IMPORTED_TARGET cairomm-1.16)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::cairomm minifb::minifb)

# fix link cairo
# https://gitlab.kitware.com/cmake/cmake/-/merge_requests/9109/diffs

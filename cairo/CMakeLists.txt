cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_cairo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_search_module(CAIROMM REQUIRED cairomm cairomm-1.16)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CAIROMM_INCLUDE_DIRS}")
target_link_libraries(${PROJECT_NAME} PRIVATE "${CAIROMM_LINK_LIBRARIES}" minifb)
-all
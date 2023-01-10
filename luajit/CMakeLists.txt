cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_luajit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(LUAJIT_INCLUDE_DIR luajit.h PATH_SUFFIXES include/luajit-2.1 include/luajit-2.0 include/luajit-5_1-2.1 include/luajit-5_1-2.0 include luajit)
find_library(LUAJIT_LIBRARY NAMES luajit-5.1 lua51 PATH_SUFFIXES lib64 lib)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${LUAJIT_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUAJIT_LIBRARY} LuaBridge)
if(APPLE)
    target_link_options(${PROJECT_NAME} PRIVATE -pagezero_size 10000)
endif()

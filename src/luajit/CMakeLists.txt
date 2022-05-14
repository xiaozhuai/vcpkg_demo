cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_luajit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(LUABRIDGE_INCLUDE_DIRS LuaBridge/LuaBridge.h)
find_package(PkgConfig REQUIRED)
pkg_check_modules(luajit REQUIRED IMPORTED_TARGET luajit)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${LUABRIDGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::luajit)
if(APPLE)
    target_link_options(${PROJECT_NAME} PRIVATE -pagezero_size 10000)
endif()

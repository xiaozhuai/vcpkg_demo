cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_protobuf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(proto)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE vcpkg_demo_protobuf_proto)

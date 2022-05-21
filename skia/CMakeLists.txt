# 设置到 3.15 以上以支持 MSVC_RUNTIME_LIBRARY PROPERTY
cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_skia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(skia REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(WebP REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        skia::skia
        PNG::PNG
        ${JPEG_LIBRARIES}
        WebP::webp WebP::webpdemux WebP::libwebpmux WebP::webpdecoder
        ZLIB::ZLIB
        minifb
)

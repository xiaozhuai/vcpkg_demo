cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_cairo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(CAIRO_LIBRARIES NAMES cairo REQUIRED)
find_library(PIXMAN_LIBRARIES NAMES pixman pixman-1 REQUIRED)
find_library(FONTCONFIG_LIBRARIES NAMES fontconfig REQUIRED)
find_library(BROTLICOMMON_LIBRARIES NAMES brotlicommon brotlicommon-static REQUIRED)
find_library(BROTLIDEC_LIBRARIES NAMES brotlidec brotlidec-static REQUIRED)
find_library(BZ2_LIBRARIES NAMES bz2 REQUIRED)
find_package(expat CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        ${CAIRO_LIBRARIES}
        ${PIXMAN_LIBRARIES}
        ${FONTCONFIG_LIBRARIES}
        ${BROTLICOMMON_LIBRARIES}
        ${BROTLIDEC_LIBRARIES}
        ${BZ2_LIBRARIES}
        expat::expat
        freetype
        PNG::PNG
        ZLIB::ZLIB
        minifb
)

if (WIN32)
    target_link_libraries(
            ${PROJECT_NAME} PRIVATE
            msimg32
    )
endif ()

# 设置到 3.15 以上以支持 MSVC_RUNTIME_LIBRARY PROPERTY
cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_opengles_angle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# msvc 支持使用无bom头的utf-8编码的源码
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(glfw3 REQUIRED)
find_package(unofficial-angle REQUIRED)
find_package(ZLIB REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
file(GLOB_RECURSE IMGUI_SOURCES imgui/*.cpp imgui/*.hpp imgui/*.c imgui/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${IMGUI_SOURCES})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set_target_properties(
            ${PROJECT_NAME} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
            # WIN32_EXECUTABLE TRUE
    )
endif ()

target_include_directories(
        ${PROJECT_NAME} PRIVATE
        ${STB_INCLUDE_DIRS}
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
)
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        glfw
        unofficial::angle::libEGL
        unofficial::angle::libANGLE
        unofficial::angle::libGLESv2
        unofficial::angle::angle_common
        ZLIB::ZLIB
        glfw
)

if (APPLE)
    target_link_libraries(
            ${PROJECT_NAME} PRIVATE
            "-framework QuartzCore"
            "-framework IOSurface"
    )
endif ()

cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_opengles_angle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
find_package(unofficial-angle REQUIRED)
find_package(ZLIB REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
file(GLOB_RECURSE IMGUI_SOURCES imgui/*.cpp imgui/*.hpp imgui/*.c imgui/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${IMGUI_SOURCES})

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
)

target_link_libraries(
        glfw PRIVATE
        unofficial::angle::libEGL
        unofficial::angle::libANGLE
        unofficial::angle::libGLESv2
        unofficial::angle::angle_common
        ZLIB::ZLIB
)
target_compile_definitions(
        glfw PRIVATE
        LINK_LIBEGL
)

if (APPLE)
    target_link_libraries(
            ${PROJECT_NAME} PRIVATE
            "-framework QuartzCore"
            "-framework IOSurface"
    )
endif ()

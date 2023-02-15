cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_opengles_angle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(unofficial-angle REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Stb REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        glfw
        imgui
        unofficial::angle::libEGL
        unofficial::angle::libGLESv2
        ZLIB::ZLIB
)

if (APPLE)
    target_link_libraries(
            ${PROJECT_NAME} PRIVATE
            "-framework QuartzCore"
            "-framework IOSurface"
    )
endif ()

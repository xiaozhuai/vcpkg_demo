cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_opencv_camera)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV CONFIG REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        opencv_core
        opencv_imgcodecs
        opencv_highgui
)

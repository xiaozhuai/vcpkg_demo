cmake_minimum_required(VERSION 3.2)
project(imgui)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE IMGUI_FILES "*.h" "*.cpp")
add_library(imgui STATIC ${IMGUI_FILES})
target_link_libraries(imgui PRIVATE glfw)
target_include_directories(imgui INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

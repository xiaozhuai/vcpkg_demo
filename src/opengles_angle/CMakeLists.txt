cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_opengles_angle)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glesx CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        glesx::GLESv2
        glfw
        imgui::imgui
        imgui::imgui_backend_gles3
        imgui::imgui_backend_glfw
)

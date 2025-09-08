cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_opengles_thorvg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glesx CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(PkgConfig)
pkg_check_modules(thorvg REQUIRED IMPORTED_TARGET thorvg)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.mm src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        glesx::EGL
        glesx::GLESv2
        glfw
        PkgConfig::thorvg
)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework QuartzCore")
endif ()

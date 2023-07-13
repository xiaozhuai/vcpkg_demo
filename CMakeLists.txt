cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_path(EGL_INCLUDE_DIR EGL/egl.h PATH_SUFFIXES include)
find_path(GLES2_INCLUDE_DIR GLES2/gl2.h PATH_SUFFIXES include)
include_directories("${EGL_INCLUDE_DIR}" "${GLES2_INCLUDE_DIR}")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_LINK_STATIC_ANGLE ON CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)

add_subdirectory(src/cairo)
add_subdirectory(src/highway)
add_subdirectory(src/imageinfo)
add_subdirectory(src/json)
add_subdirectory(src/libusb)
add_subdirectory(src/libyuv)
add_subdirectory(src/luajit)
add_subdirectory(src/opencv)
add_subdirectory(src/opencv_camera)
add_subdirectory(src/opencv_scanner)
add_subdirectory(src/opencv_segment)
add_subdirectory(src/opencv_verification_code)
add_subdirectory(src/opengles_angle)
add_subdirectory(src/protobuf)
add_subdirectory(src/qt5_quick)
add_subdirectory(src/qt5_widgets)
add_subdirectory(src/sigslot)
add_subdirectory(src/skia)
add_subdirectory(src/tbb)
add_subdirectory(src/uvw)
add_subdirectory(src/xorstr)
add_subdirectory(src/yoga)

file(GLOB_RECURSE ALL_SOURCES "src/*.h" "src/*.hpp" "src/*.c" "src/*.cpp" "src/*.cc")
find_program(CLANG_FORMAT clang-format)
if (CLANG_FORMAT)
    add_custom_target(
            format
            COMMAND "${CLANG_FORMAT}" -i -verbose ${ALL_SOURCES}
            WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    )
endif ()

cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(src/cairo)
add_subdirectory(src/duktape)
add_subdirectory(src/highway)
add_subdirectory(src/imageinfo)
add_subdirectory(src/imobiledevice)
add_subdirectory(src/json)
add_subdirectory(src/leveldb)
add_subdirectory(src/libusb)
add_subdirectory(src/libyuv)
add_subdirectory(src/luajit)
add_subdirectory(src/opencv)
add_subdirectory(src/opencv_camera)
add_subdirectory(src/opencv_hair_removal)
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

find_program(CLANG_FORMAT clang-format)
if (CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS "src/*.h" "src/*.hpp" "src/*.c" "src/*.cpp" "src/*.cc")
    add_custom_target(
            format
            COMMAND "${CLANG_FORMAT}" -i --verbose ${ALL_SOURCES}
            WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    )
endif ()

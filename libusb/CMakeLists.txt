cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_libusb)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(LIBUSB_INCLUDE_DIRS NAMES libusb.h PATH_SUFFIXES libusb-1.0 REQUIRED)
find_library(LIBUSB_LIBRARIES NAMES usb usb-1.0 REQUIRED)
add_library(libusb INTERFACE)
target_include_directories(libusb INTERFACE ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(libusb INTERFACE ${LIBUSB_LIBRARIES})
if (APPLE)
    find_library(CoreFoundation CoreFoundation REQUIRED)
    find_library(IOKit IOKit REQUIRED)
    find_library(Security Security REQUIRED)
    target_link_libraries(libusb INTERFACE ${CoreFoundation} ${IOKit} ${Security})
endif ()

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE libusb)

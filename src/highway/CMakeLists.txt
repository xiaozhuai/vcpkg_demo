cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_highway)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(hwy CONFIG REQUIRED)
find_package(nanobench CONFIG REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE hwy::hwy nanobench::nanobench)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    if(MSVC)
        string(REGEX REPLACE "/RTC[^ ]*" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
        set_source_files_properties(src/task.cpp src/task.h PROPERTIES COMPILE_FLAGS "/O2 /Ob1")
    else()
        set_source_files_properties(src/task.cpp src/task.h PROPERTIES COMPILE_FLAGS "-O2")
    endif()
endif()

cmake_minimum_required(VERSION 3.15)

project(vcpkg_demo_highway)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(HIGHWAY_INCLUDE_DIRS hwy/highway.h)
find_library(HIGHWAY_LIBRARY NAMES hwy PATH_SUFFIXES lib)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${HIGHWAY_INCLUDE_DIRS} src)
target_link_libraries(${PROJECT_NAME} PRIVATE ${HIGHWAY_LIBRARY})
if(MSVC)
    set_source_files_properties(src/task.cpp src/task.h PROPERTIES COMPILE_FLAGS /O2)
else()
    set_source_files_properties(src/task.cpp src/task.h PROPERTIES COMPILE_FLAGS -O2)
endif()

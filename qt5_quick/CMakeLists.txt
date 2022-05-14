# 设置到 3.15 以上以支持 MSVC_RUNTIME_LIBRARY PROPERTY
cmake_minimum_required(VERSION 3.15)

# 设置triplet和toolchain，必须放在project之前
if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set(VCPKG_TARGET_TRIPLET x64-windows-static-mt-release)
else ()
    set(VCPKG_TARGET_TRIPLET x64-windows)
endif ()
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE_TOOLCHAIN_FILE}" CACHE STRING "vcpkg toolchain file")

project(
        vcpkg_demo_qt5_quick
        VERSION 0.1
        DESCRIPTION "A demo project with vcpkg and qt5"
        LANGUAGES CXX
)
set(PROJECT_GUI_IDENTIFIER net.xiaozhuai.vcpkg_demo_qt)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# msvc 支持使用无bom头的utf-8编码的源码
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(Qt5 COMPONENTS Core Quick QuickControls2 QmlImportScanner REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.hpp src/*.c src/*.h src/*.qrc)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::QuickControls2)

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_GUI_IDENTIFIER}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set_target_properties(
            ${PROJECT_NAME} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
            # WIN32_EXECUTABLE TRUE
    )
endif ()

qt5_import_qml_plugins(${PROJECT_NAME})

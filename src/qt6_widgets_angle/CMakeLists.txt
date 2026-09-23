cmake_minimum_required(VERSION 3.20)

project(
    vcpkg_demo_qt6_widgets_angle
    VERSION 0.1
    DESCRIPTION "A demo project with vcpkg and qt6 and angle"
    LANGUAGES CXX
)
set(PROJECT_GUI_IDENTIFIER net.xiaozhuai.vcpkg_demo_qt6_widgets_angle)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
find_package(glesx CONFIG REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp src/*.c src/*.h)
qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

file(GLOB_RECURSE PROJECT_RESOURCES CONFIGURE_DEPENDS assets/*)
qt_add_resources(${PROJECT_NAME} "assets"
    PREFIX "/"
    BASE "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    FILES ${PROJECT_RESOURCES}
)

file(GLOB_RECURSE PROJECT_UI_FILES CONFIGURE_DEPENDS src/*.ui)
qt_add_ui(${PROJECT_NAME} SOURCES ${PROJECT_UI_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    glesx::EGL
    glesx::GLESv2
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_GUI_IDENTIFIER}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    # WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(${PROJECT_NAME})

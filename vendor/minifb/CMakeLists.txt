cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME MiniFB)
project(${PROJECT_NAME})

message(STATUS "Processing " ${PROJECT_NAME})

# Detect iOS
#--------------------------------------
if(NOT DEFINED IOS)
    if(DEFINED CMAKE_SYSTEM_NAME)
        string(TOLOWER CMAKE_SYSTEM_NAME CMAKE_SYSTEM_NAME_LOWER)
        if(CMAKE_SYSTEM_NAME_LOWER STREQUAL "ios")
            set(IOS true)
        endif()
    endif()
endif()

# Sources
#--------------------------------------
set(SrcLib
    include/MiniFB.h
    include/MiniFB_cpp.h
    include/MiniFB_enums.h

    src/MiniFB_common.c
    src/MiniFB_cpp.cpp
    src/MiniFB_internal.c
    src/MiniFB_internal.h
    src/MiniFB_timer.c
    src/WindowData.h
)

#--
set(SrcWindows
    src/windows/WinMiniFB.c
    src/windows/WindowData_Win.h
)

#--
set(SrcMacOSX
    src/macosx/MacMiniFB.m
    src/macosx/OSXWindow.h
    src/macosx/OSXWindow.m
    src/macosx/OSXView.h
    src/macosx/OSXView.m
    src/macosx/OSXViewDelegate.h
    src/macosx/OSXViewDelegate.m
    src/macosx/WindowData_OSX.h
)

#--
set(SrcIOS
    src/ios/WindowData_IOS.h
    src/ios/iOSMiniFB.m
    src/ios/iOSView.h
    src/ios/iOSView.m
    src/ios/iOSViewController.h
    src/ios/iOSViewController.m
    src/ios/iOSViewDelegate.h
    src/ios/iOSViewDelegate.m
    include/MiniFB_ios.h
)

#--
set(SrcWayland
    src/wayland/WaylandMiniFB.c
    src/wayland/WindowData_Way.h
    src/MiniFB_linux.c
)

#--
set(SrcX11
    src/x11/X11MiniFB.c
    src/x11/WindowData_X11.h
    src/MiniFB_linux.c
)

set(SrcGL
    src/gl/MiniFB_GL.h
    src/gl/MiniFB_GL.c
)


# Set features
#--------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#--------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(APPLE AND NOT IOS)
    option(USE_METAL_API "Build the project using metal API code" ON)
    option(USE_INVERTED_Y_ON_MACOS "Use default mouse position: (0, 0) at (left, down)" OFF)
elseif(UNIX)
    option(USE_WAYLAND_API "Build the project using wayland API code" OFF)
    if(NOT USE_WAYLAND_API)
        option(USE_OPENGL_API "Build the project using OpenGL API code" ON)
    endif()
elseif(WIN32)
    option(USE_OPENGL_API "Build the project using OpenGL API code" ON)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
    add_definitions(-DDEBUG)
endif()

# Set compiler/platform specific flags and dependencies
#--------------------------------------
if(WIN32)

    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()
    add_definitions(-D_WIN32_WINNT=0x0601)  # Windows 7 (we are in 2020)

    if(USE_OPENGL_API)
        list(APPEND SrcLib ${SrcGL})

        add_definitions(-DUSE_OPENGL_API)
    endif()

    list(APPEND SrcLib ${SrcWindows})

elseif(IOS)

    list(APPEND SrcLib ${SrcIOS})

elseif(APPLE)

    if(USE_METAL_API)
        add_definitions(-DUSE_METAL_API)
    endif()

    if(USE_INVERTED_Y_ON_MACOS)
        add_definitions(-DUSE_INVERTED_Y_ON_MACOS)
    endif()

    list(APPEND SrcLib ${SrcMacOSX})

elseif(UNIX)

    if(USE_WAYLAND_API)
        list(APPEND SrcLib ${SrcWayland})
    else()
        if(USE_OPENGL_API)
            list(APPEND SrcLib ${SrcGL})

            add_definitions(-DUSE_OPENGL_API)
        endif()
        list(APPEND SrcLib ${SrcX11})
    endif()

endif()

# Library
#--------------------------------------
add_library(minifb STATIC
    ${SrcLib}
)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set_target_properties(
            minifb PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif ()

# Link
#--------------------------------------
if(APPLE)

    if(IOS)
        target_link_libraries(minifb
            "-framework UIKit"
            "-framework QuartzCore"
            "-framework Metal"
            "-framework MetalKit"
        )
    else()
        target_link_libraries(minifb
            "-framework Cocoa"
            "-framework QuartzCore"
            "-framework Metal"
            "-framework MetalKit"
        )
    endif()

elseif(UNIX)

    if(USE_WAYLAND_API)
        target_link_libraries(minifb
            "-lwayland-client"
            "-lwayland-cursor"
        )
    else()
        target_link_libraries(minifb
            "-lX11"
            #"-lxkbcommon"
            #"-lXrandr" DPI NOT WORKING
        )
        if(USE_OPENGL_API)
        target_link_libraries(minifb
            "-lGL"
        )
        endif()
    endif()

elseif(WIN32)

    if(USE_OPENGL_API)
        target_link_libraries(minifb
            "Opengl32.lib"
        )
    endif()

    target_link_libraries(minifb
        "winmm.lib"
    )

endif()

# For all projects
#--------------------------------------
target_include_directories(minifb PUBLIC  ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(minifb PRIVATE src)

# 设置triplet和toolchain，必须放在project之前include
# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# include(vcpkg_toolchain)

cmake_minimum_required(VERSION 3.15)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set(IS_STATIC_RELEASE TRUE)
else ()
    set(IS_STATIC_RELEASE FALSE)
endif ()

option(IS_64BIT "Is 64 bit" TRUE)

if (WIN32)
    # msvc 支持使用无bom头的utf-8编码的源码
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    # 设置flags
    set(CMAKE_C_FLAGS_DEBUG "/Zi /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Ob0 /Od /RTC1")
    set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /DNDEBUG")
    set(CMAKE_C_FLAGS_MINSIZEREL "/O2 /Ob2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/O2 /Ob2 /DNDEBUG")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/Zi /O2 /Ob1 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /O2 /Ob1 /DNDEBUG")
    # 设置triplet
    if (IS_64BIT)
        if (IS_STATIC_RELEASE)
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
            set(VCPKG_TARGET_TRIPLET x64-windows-static-mt-release)
        else ()
            set(VCPKG_TARGET_TRIPLET x64-windows)
        endif ()
    else ()
        if (IS_STATIC_RELEASE)
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
            set(VCPKG_TARGET_TRIPLET x86-windows-static-mt-release)
        else ()
            set(VCPKG_TARGET_TRIPLET x86-windows)
        endif ()
    endif ()
elseif (APPLE)
    # 设置flags
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_MINSIZEREL "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-O3 -DNDEBUG")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
    # 设置目标系统版本
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
    # 设置triplet
    if (IS_STATIC_RELEASE)
        set(VCPKG_TARGET_TRIPLET x64-osx-catalina-static-release)
    else ()
        set(VCPKG_TARGET_TRIPLET x64-osx-catalina)
    endif ()
else ()
    message(FATAL_ERROR "Unsupported ${CMAKE_SYSTEM_NAME}")
endif ()

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELWITHDEBINFO)
foreach (CompilerFlag ${CompilerFlags})
    message(STATUS "Flags: ${CompilerFlag}: ${${CompilerFlag}}")
endforeach ()

if ("$ENV{VCPKG_CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
    message(FATAL_ERROR "Env VCPKG_CMAKE_TOOLCHAIN_FILE not set")
endif ()

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE_TOOLCHAIN_FILE}" CACHE STRING "vcpkg toolchain file")

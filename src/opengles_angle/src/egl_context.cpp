//
// Copyright (c) 2026 xiaozhuai
//

#include "egl_context.hpp"

#include <cstdio>
#include <string>
#include <string_view>

#include "EGL/eglext.h"

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#include "EGL/eglext_angle.h"
#endif

#if defined(__APPLE__)
#include <objc/message.h>
#include <objc/objc.h>
#include <objc/runtime.h>
template <typename T, typename... Args>
T objc_call(id obj, const char *sel, Args... args) {
    using FuncPtr = T (*)(id, SEL, Args...);
    return reinterpret_cast<FuncPtr>(objc_msgSend)(obj, sel_registerName(sel), args...);
}
template <typename T, typename... Args>
T objc_call(const char *clazz, const char *sel, Args... args) {
    return objc_call<T>(reinterpret_cast<id>(objc_getClass(clazz)), sel, args...);
}
template <typename T, typename... Args>
T objc_call(Class clazz, const char *sel, Args... args) {
    return objc_call<T>(reinterpret_cast<id>(clazz), sel, args...);
}
id objc_ns_string(const char *str) { return objc_call<id, const char *>("NSString", "stringWithUTF8String:", str); }
const char *objc_ns_string_cstr(id ns_str) { return objc_call<const char *>(ns_str, "UTF8String"); }
#define GLFW_EXPOSE_NATIVE_COCOA
#include "EGL/eglext_angle.h"
#endif

#if defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif

#if defined(__EMSCRIPTEN__)
#include "emscripten/html5.h"
#else
#include "GLFW/glfw3native.h"
#endif

EGLDisplay initEglDisplay() {
#if defined(__APPLE__)
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_METAL_ANGLE,
        EGL_POWER_PREFERENCE_ANGLE,
        EGL_HIGH_POWER_ANGLE,
        EGL_NONE,
    };
    EGLDisplay display = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, nullptr, display_attribs);
#elif defined(_WIN32)
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
        EGL_NONE,
    };
    EGLDisplay display = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, nullptr, display_attribs);
#elif defined(__linux__)
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#else
#error "Unsupported platform for EGL display initialization"
#endif
    if (display == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to get EGL display, error: 0x%X\n", eglGetError());
        std::abort();
    }
    if (!eglInitialize(display, nullptr, nullptr)) {
        fprintf(stderr, "Failed to initialize EGL, error: 0x%X\n", eglGetError());
        std::abort();
    }

    return display;
}

void destroyEglDisplay(EGLDisplay display) {
    if (display != EGL_NO_DISPLAY) {
        eglTerminate(display);
    }
}

EGLConfig initEglConfig(EGLDisplay display) {
    // clang-format off
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE,          EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,       EGL_OPENGL_ES3_BIT,
        EGL_COLOR_BUFFER_TYPE,     EGL_RGB_BUFFER,
        EGL_BUFFER_SIZE,           32,
        EGL_RED_SIZE,              8,
        EGL_GREEN_SIZE,            8,
        EGL_BLUE_SIZE,             8,
        EGL_ALPHA_SIZE,            8,
        EGL_DEPTH_SIZE,            24,
        EGL_STENCIL_SIZE,          8,
        EGL_NONE
    };
    // clang-format on
    EGLint num_configs;
    EGLConfig config = nullptr;
    if (!eglChooseConfig(display, config_attribs, &config, 1, &num_configs)) {
        fprintf(stderr, "Failed to choose EGL config, error: 0x%X\n", eglGetError());
        exit(1);
    }
    return config;
}

EGLContext initEglContext(EGLDisplay display, EGLConfig config) {
    // clang-format off
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    // clang-format on
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);
    if (!context) {
        fprintf(stderr, "Failed to create EGL context, error: 0x%X\n", eglGetError());
        exit(1);
    }
    return context;
}

void destroyEglContext(EGLDisplay display, EGLContext context) {
    if (context != EGL_NO_CONTEXT) {
        eglDestroyContext(display, context);
    }
}

EGLSurface createEglSurface(EGLDisplay display, EGLConfig config, GLFWwindow *window) {
#if defined(__APPLE__)
    Class AutoResetScaleFactorLayerClass = objc_getClass("AutoResetScaleFactorLayer");
    if (!AutoResetScaleFactorLayerClass) {
        Class CAMetalLayerClass = objc_getClass("CAMetalLayer");
        AutoResetScaleFactorLayerClass = objc_allocateClassPair(CAMetalLayerClass, "AutoResetScaleFactorLayer", 0);
        using MethodType = void (*)(id self, SEL, id key_path, id object, id, void *);
        MethodType observe_value_for_key_path = [](id self, SEL, id key_path, id object, id, void *) {
            std::string_view key_path_str = objc_ns_string_cstr(key_path);
            if (key_path_str == "backingScaleFactor") {
                auto scale_factor = objc_call<CGFloat>(object, "backingScaleFactor");
                objc_call<void, CGFloat>(self, "setContentsScale:", scale_factor);
            }
        };
        class_addMethod(AutoResetScaleFactorLayerClass,
                        sel_registerName("observeValueForKeyPath:ofObject:change:context:"),
                        reinterpret_cast<IMP>(observe_value_for_key_path), "v@:@@@^v");
        objc_registerClassPair(AutoResetScaleFactorLayerClass);
    }
    auto ns_window = glfwGetCocoaWindow(window);
    CFRetain(ns_window);
    auto view = objc_call<id>(ns_window, "contentView");
    CFRetain(view);
    objc_call<void, BOOL>(view, "setWantsLayer:", YES);
    auto layer = objc_call<id>(AutoResetScaleFactorLayerClass, "layer");
    auto scale_factor = objc_call<CGFloat>(ns_window, "backingScaleFactor");
    objc_call<void, CGFloat>(layer, "setContentsScale:", scale_factor);
    objc_call<void, id, id, uint, id>(ns_window, "addObserver:forKeyPath:options:context:", layer,
                                      objc_ns_string("backingScaleFactor"), 0x01, nullptr);
    objc_call<void, id>(view, "setLayer:", layer);
    auto surface = eglCreateWindowSurface(display, config, layer, nullptr);
    CFRelease(view);
    CFRelease(ns_window);
    return surface;
#elif defined(_WIN32)
    return eglCreateWindowSurface(display, config, glfwGetWin32Window(window), nullptr);
#elif defined(__linux__)
    return eglCreateWindowSurface(display, config, glfwGetX11Window(window), nullptr);
#elif defined(__EMSCRIPTEN__)
    auto webgl_context = emscripten_webgl_get_current_context();
    return eglCreateWindowSurface(display, config, (EGLNativeWindowType)webgl_context, nullptr);
#endif
}

void destroyEglSurface(EGLDisplay display, EGLSurface surface) {
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display, surface);
    }
}

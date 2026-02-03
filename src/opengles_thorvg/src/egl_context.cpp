//
// Copyright (c) 2026 xiaozhuai
//

#include "egl_context.hpp"

#include <cstdio>
#include <cstdlib>

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

void destroyEglSurface(EGLDisplay display, EGLSurface surface) {
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display, surface);
    }
}

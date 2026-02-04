//
// Copyright (c) 2026 xiaozhuai
//

#pragma once
#ifndef VCPKG_DEMO_EGL_CONTEXT_HPP
#define VCPKG_DEMO_EGL_CONTEXT_HPP

#include "EGL/egl.h"
#include "GLFW/glfw3.h"

EGLDisplay initEglDisplay();

void destroyEglDisplay(EGLDisplay display);

EGLConfig initEglConfig(EGLDisplay display);

EGLContext initEglContext(EGLDisplay display, EGLConfig config);

void destroyEglContext(EGLDisplay display, EGLContext context);

EGLSurface createEglSurface(EGLDisplay display, EGLConfig config, GLFWwindow *window);

void destroyEglSurface(EGLDisplay display, EGLSurface surface);

#endif  // VCPKG_DEMO_EGL_CONTEXT_HPP

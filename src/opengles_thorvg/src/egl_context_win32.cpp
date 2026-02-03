#if defined(_WIN32)

#define GLFW_EXPOSE_NATIVE_WIN32

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "egl_context.hpp"

EGLSurface createEglSurface(EGLDisplay display, EGLConfig config, GLFWwindow *window) {
    return eglCreateWindowSurface(display, config, glfwGetWin32Window(window), nullptr);
}

#endif

#if defined(_WIN32)

#define GLFW_EXPOSE_NATIVE_WIN32

#include "EGL/egl.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

EGLSurface createEGLSurface(EGLDisplay display, EGLConfig config, GLFWwindow *window) {
    return eglCreateWindowSurface(display, config, glfwGetWin32Window(window), nullptr);
}

#endif

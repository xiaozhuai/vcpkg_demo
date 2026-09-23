//
// Copyright (c) 2026 xiaozhuai
//

#pragma once
#ifndef VCPKG_DEMO_RENDERER_HPP
#define VCPKG_DEMO_RENDERER_HPP

#include <GLES3/gl3.h>

#include <atomic>
#include <thread>

#include "RenderContext.h"

class RenderWindow;

class Renderer final {
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&other) = delete;
    Renderer &operator=(Renderer &&other) = delete;

    void start(RenderWindow *window);
    void stop();
    void render();

private:
    RenderContext renderContext;
    std::atomic_bool rendering{false};
    std::thread renderThread;
    EGLSurface surface = EGL_NO_SURFACE;
    GLuint program = 0;
};

#endif  // VCPKG_DEMO_RENDERER_HPP

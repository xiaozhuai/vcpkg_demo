//
// Copyright (c) 2026 xiaozhuai
//

#pragma once
#ifndef VCPKG_DEMO_RENDERCONTEXT_H
#define VCPKG_DEMO_RENDERCONTEXT_H

#include <EGL/egl.h>

class RenderWindow;

class RenderContext final {
public:
    RenderContext();
    ~RenderContext();

    RenderContext(const RenderContext &) = delete;
    RenderContext &operator=(const RenderContext &) = delete;
    RenderContext(RenderContext &&other) noexcept;
    RenderContext &operator=(RenderContext &&other) noexcept;

    [[nodiscard]] bool inited() const { return display_ != EGL_NO_DISPLAY && context_ != EGL_NO_CONTEXT; }

    void init();

    void init(const RenderContext &shared_from);

    void destroy();

    EGLSurface createSurface(RenderWindow *window);

    void destroySurface(EGLSurface surface);

    void getSurfaceSize(EGLSurface surface, int &width, int &height);

    void makeCurrent(EGLSurface surface);

    void makeCurrent(EGLSurface draw_surface, EGLSurface read_surface);

    void doneCurrent();

    void setSwapInterval(int interval);

    void swapBuffers(EGLSurface surface);

    [[nodiscard]] EGLDisplay display() const { return display_; }
    [[nodiscard]] EGLConfig config() const { return display_; }
    [[nodiscard]] EGLContext context() const { return context_; }

private:
    bool isSharedFromOther_ = false;
    EGLDisplay display_ = EGL_NO_DISPLAY;
    EGLConfig config_ = nullptr;
    EGLContext context_ = EGL_NO_CONTEXT;
};

#endif  // VCPKG_DEMO_RENDERCONTEXT_H

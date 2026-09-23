//
// Copyright (c) 2026 xiaozhuai
//

#define EGL_EGLEXT_PROTOTYPES

#include "RenderContext.h"

#include <EGL/eglext.h>
#include <EGL/eglext_angle.h>

#include "RenderWindow.h"

#define GLPP_ASSERT(...)

#if defined(_WIN32)
#define ANGLE_BACKEND_D3D11
#endif

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
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
#define ANGLE_BACKEND_METAL
#endif

#if defined(_WIN32)
// Use discrete GPU by default.
extern "C" {
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
__declspec(dllexport) DWORD NvOptimusEnablement;
// http://developer.amd.com/community/blog/2015/10/02/amd-enduro-system-for-developers/
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance;
}
static void win32_use_high_performance_gpu() {
    NvOptimusEnablement = 1;                   // Enable NVIDIA GPU
    AmdPowerXpressRequestHighPerformance = 1;  // Enable AMD GPU
}
#endif

RenderContext::RenderContext() = default;

RenderContext::~RenderContext() { destroy(); }

RenderContext::RenderContext(RenderContext &&other) noexcept {
    isSharedFromOther_ = other.isSharedFromOther_;
    display_ = other.display_;
    config_ = other.config_;
    context_ = other.context_;
    other.isSharedFromOther_ = false;
    other.display_ = EGL_NO_DISPLAY;
    other.config_ = nullptr;
    other.context_ = EGL_NO_CONTEXT;
}

RenderContext &RenderContext::operator=(RenderContext &&other) noexcept {
    if (this != &other) {
        destroy();
        isSharedFromOther_ = other.isSharedFromOther_;
        display_ = other.display_;
        config_ = other.config_;
        context_ = other.context_;
        other.isSharedFromOther_ = false;
        other.display_ = EGL_NO_DISPLAY;
        other.config_ = nullptr;
        other.context_ = EGL_NO_CONTEXT;
    }
    return *this;
}

void RenderContext::init() {
    isSharedFromOther_ = false;
#if defined(_WIN32)
    win32_use_high_performance_gpu();
#endif
#if defined(__APPLE__)
#if defined(ANGLE_BACKEND_METAL)
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_METAL_ANGLE,
        EGL_POWER_PREFERENCE_ANGLE,
        EGL_HIGH_POWER_ANGLE,
        EGL_NONE,
    };
#elif defined(ANGLE_BACKEND_OPENGL)
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE,
        EGL_POWER_PREFERENCE_ANGLE,
        EGL_HIGH_POWER_ANGLE,
        EGL_NONE,
    };
#else
    EGLAttrib display_attribs[] = {
        EGL_NONE,
    };
#error "No angle backend specified"
#endif
    display_ = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, nullptr, display_attribs);
#elif defined(_WIN32)
#if defined(ANGLE_BACKEND_D3D11)
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
        EGL_NONE,
    };
#elif defined(ANGLE_BACKEND_VULKAN)
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_VULKAN_ANGLE,
        EGL_NONE,
    };
#elif defined(ANGLE_BACKEND_D3D11ON12)
    auto dyn_eglSetValidationEnabledANGLE =
        reinterpret_cast<PFNEGLSETVALIDATIONENABLEDANGLEPROC>(eglGetProcAddress("eglSetValidationEnabledANGLE"));
    GLPP_ASSERT(dyn_eglSetValidationEnabledANGLE, "eglSetValidationEnabledANGLE is not supported");
    dyn_eglSetValidationEnabledANGLE(EGL_FALSE);
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
        EGL_PLATFORM_ANGLE_D3D11ON12_ANGLE,
        EGL_TRUE,
        EGL_NONE,
    };
#elif defined(ANGLE_BACKEND_OPENGL)
    EGLAttrib display_attribs[] = {
        EGL_PLATFORM_ANGLE_TYPE_ANGLE,
        EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE,
        EGL_NONE,
    };
#else
    EGLAttrib display_attribs[] = {
        EGL_NONE,
    };
#error "No angle backend specified"
#endif
    display_ = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, nullptr, display_attribs);
#else
#error "Unsupported platform"
#endif
    GLPP_ASSERT(display_ != EGL_NO_DISPLAY, "Failed to get EGL display, error: 0x{:X}", eglGetError());
    EGLBoolean result = EGL_FALSE;
    result = eglInitialize(display_, nullptr, nullptr);
    GLPP_ASSERT(result, "Failed to initialize EGL, error: 0x{:X}", eglGetError());

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
        EGL_NONE,
    };
    // clang-format on
    EGLint num_configs;
    result = eglChooseConfig(display_, config_attribs, &config_, 1, &num_configs);
    GLPP_ASSERT(result, "Failed to choose EGL config, error: 0x{:X}", eglGetError());

    // clang-format off
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE,
    };
    // clang-format on
    context_ = eglCreateContext(display_, config_, EGL_NO_CONTEXT, context_attribs);
    GLPP_ASSERT(context_ != EGL_NO_CONTEXT, "Failed to create EGL context, error: 0x{:X}", eglGetError());
}

void RenderContext::init(const RenderContext &shared_from) {
    isSharedFromOther_ = true;
    display_ = shared_from.display_;
    config_ = shared_from.config_;

    // clang-format off
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE,
    };
    // clang-format on
    context_ = eglCreateContext(display_, config_, shared_from.context_, context_attribs);
    GLPP_ASSERT(context_ != EGL_NO_CONTEXT, "Failed to create EGL context, error: 0x{:X}", eglGetError());
}

void RenderContext::destroy() {
    if (context_ != EGL_NO_CONTEXT) {
        eglDestroyContext(display_, context_);
    }
    if (display_ != EGL_NO_DISPLAY && !isSharedFromOther_) {
        eglTerminate(display_);
    }
    isSharedFromOther_ = false;
    context_ = EGL_NO_CONTEXT;
    config_ = nullptr;
    display_ = EGL_NO_DISPLAY;
}

EGLSurface RenderContext::createSurface(RenderWindow *window) {
#if defined(__APPLE__)
    auto view = reinterpret_cast<id>(window->winId());
    auto layer = objc_call<id>(view, "layer");
    QObject::connect(window, &RenderWindow::devicePixelRatioChanged, [window](qreal scale_factor) {
        auto view = reinterpret_cast<id>(window->winId());
        auto layer = objc_call<id>(view, "layer");
        objc_call<void, CGFloat>(layer, "setContentsScale:", static_cast<CGFloat>(scale_factor));
    });
    auto scale_factor = objc_call<CGFloat>(view, "backingScaleFactor");
    objc_call<void, CGFloat>(layer, "setContentsScale:", scale_factor);
    return eglCreatePlatformWindowSurface(display_, config_, layer, nullptr);
#elif defined(_WIN32)
    return eglCreatePlatformWindowSurface(display_, config_, (EGLNativeWindowType)window->winId(), nullptr);
#else
#error "Unsupported platform"
#endif
}

void RenderContext::destroySurface(EGLSurface surface) {
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display_, surface);
    }
}

void RenderContext::getSurfaceSize(EGLSurface surface, int &width, int &height) {
    eglQuerySurface(display_, surface, EGL_WIDTH, &width);
    eglQuerySurface(display_, surface, EGL_HEIGHT, &height);
}

void RenderContext::makeCurrent(EGLSurface surface) { eglMakeCurrent(display_, surface, surface, context_); }

void RenderContext::makeCurrent(EGLSurface draw_surface, EGLSurface read_surface) {
    eglMakeCurrent(display_, draw_surface, read_surface, context_);
}

void RenderContext::doneCurrent() { eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT); }

void RenderContext::setSwapInterval(int interval) { eglSwapInterval(display_, interval); }

void RenderContext::swapBuffers(EGLSurface surface) { eglSwapBuffers(display_, surface); }

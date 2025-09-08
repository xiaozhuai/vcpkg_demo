/**
 * Copyright 2022 xiaozhuai
 */

#define GLFW_INCLUDE_NONE

#include <cstdio>
#include <memory>
#include <string>

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "EGL/eglext_angle.h"
#include "GLES3/gl3.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "thorvg.h"

bool showImgui = true;

static void glfwErrorCallback(int code, const char *msg) { fprintf(stderr, "[%d] %s\n", code, msg); }

static void glfwKeyCallback(GLFWwindow *win, int key, int scanCode, int action, int mods) {
    if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) {
        showImgui = !showImgui;
    } else if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

GLFWwindow *initWindow() {
    if (!glfwInit()) {
        fprintf(stderr, "Could not init GLFW\n");
        exit(1);
    }
    glfwSetErrorCallback(glfwErrorCallback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    auto *window = glfwCreateWindow(960, 540, "vcpkg_demo_opengles_angle", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Could not create window\n");
        glfwTerminate();
        exit(1);
    }
    glfwSetKeyCallback(window, glfwKeyCallback);
    return window;
}

EGLDisplay initEGLDisplay() {
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
        exit(1);
    }
    if (!eglInitialize(display, nullptr, nullptr)) {
        fprintf(stderr, "Failed to initialize EGL, error: 0x%X\n", eglGetError());
        exit(1);
    }

    return display;
}

void destroyEGLDisplay(EGLDisplay display) {
    if (display != EGL_NO_DISPLAY) {
        eglTerminate(display);
    }
}

EGLConfig initEGLConfig(EGLDisplay display) {
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

EGLContext initEGLContext(EGLDisplay display, EGLConfig config) {
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

void destroyEGLContext(EGLDisplay display, EGLContext context) {
    if (context != EGL_NO_CONTEXT) {
        eglDestroyContext(display, context);
    }
}

EGLSurface createEGLSurface(EGLDisplay display, EGLConfig config, GLFWwindow *window);

void destroyEGLSurface(EGLDisplay display, EGLSurface surface) {
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display, surface);
    }
}

GLuint createShader(GLenum type, const std::string &source) {
    auto shader = glCreateShader(type);
    if (shader == 0) {
        fprintf(stderr, "Error create shader, %s\n", source.c_str());
        exit(1);
    }
    const char *src = source.c_str();
    int len = static_cast<int>(source.size());
    glShaderSource(shader, 1, &src, &len);
    glCompileShader(shader);
    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLsizei length;
        char log[GL_INFO_LOG_LENGTH] = {0};
        glGetShaderInfoLog(shader, GL_INFO_LOG_LENGTH, &length, log);
        fprintf(stderr, "Error compile shader, %s\n%s\n", source.c_str(), log);
        exit(1);
    }
    return shader;
}

GLuint createProgram(const std::string &vert, const std::string &frag) {
    auto vs = createShader(GL_VERTEX_SHADER, vert);
    auto fs = createShader(GL_FRAGMENT_SHADER, frag);
    auto program = glCreateProgram();
    if (program == 0) {
        fprintf(stderr, "Error create program\n");
        exit(1);
    }
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei length;
        char log[GL_INFO_LOG_LENGTH] = {0};
        glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, &length, log);
        fprintf(stderr, "Error link program\n%s\n", log);
        exit(1);
    }
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

GLuint createTexture(int width, int height, const void *pixels) {
    GLuint texture;
    glGenTextures(1, &texture);
    if (texture == 0) {
        fprintf(stderr, "Error create texture\n");
        exit(1);
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    return texture;
}

GLuint createFramebuffer(int width, int height, GLuint texture) {
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    if (framebuffer == 0) {
        fprintf(stderr, "Error create framebuffer\n");
        exit(1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Framebuffer is not complete\n");
        exit(1);
    }
    return framebuffer;
}

GLuint createTextureFromFile(const std::string &file) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t *pixels = stbi_load(file.c_str(), &width, &height, &channels, 4);
    GLuint texture = createTexture(width, height, pixels);
    stbi_image_free(pixels);
    return texture;
}

int main() {
    const char *VERT = R"(
attribute vec4 aPosition;
attribute vec2 aTexCoord;
varying vec2 texCoord;
void main() {
    gl_Position = aPosition;
    texCoord = aTexCoord;
}
)";
    const char *FRAG = R"(
varying highp vec2 texCoord;
uniform sampler2D uTexture;
void main() {
    gl_FragColor = texture2D(uTexture, texCoord);
}
)";
    GLuint program = 0;
    GLuint texture = 0;
    GLuint aPosition;
    GLuint aTexCoord;
    GLint uTexture;
    float vertices[] = {
        // clang-format off
        -1.0f, -1.0f, 0.0f, 0.0f,
        +1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, +1.0f, 0.0f, 1.0f,
        +1.0f, +1.0f, 1.0f, 1.0f,
        // clang-format on
    };
    uint8_t indices[] = {0, 1, 2, 2, 1, 3};
    int fbWidth = 0;
    int fbHeight = 0;

    auto *window = initWindow();

    auto egl_display = initEGLDisplay();
    auto egl_config = initEGLConfig(egl_display);
    auto egl_context = initEGLContext(egl_display, egl_config);
    auto egl_surface = createEGLSurface(egl_display, egl_config, window);
    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    int tvg_canvas_width = 400;
    int tvg_canvas_height = 400;
    GLuint tvg_texture = createTexture(tvg_canvas_width, tvg_canvas_height, nullptr);
    GLuint tvg_framebuffer = createFramebuffer(tvg_canvas_width, tvg_canvas_height, tvg_texture);

    tvg::Initializer::init(tvg::CanvasEngine::Gl, 4);
    auto tvg_canvas = tvg::GlCanvas::gen();
    tvg_canvas->target(tvg_framebuffer, tvg_canvas_width, tvg_canvas_height);

    program = createProgram(VERT, FRAG);
    texture = createTextureFromFile("assets/test.jpg");
    glUseProgram(program);
    aPosition = glGetAttribLocation(program, "aPosition");
    aTexCoord = glGetAttribLocation(program, "aTexCoord");
    uTexture = glGetUniformLocation(program, "uTexture");

    while (!glfwWindowShouldClose(window)) {
        eglQuerySurface(egl_display, egl_surface, EGL_WIDTH, &fbWidth);
        eglQuerySurface(egl_display, egl_surface, EGL_HEIGHT, &fbHeight);

        glfwPollEvents();

        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, fbWidth, fbHeight);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glUseProgram(program);
            glVertexAttribPointer(aPosition, 2, GL_FLOAT, false, 4 * sizeof(float), vertices);
            glEnableVertexAttribArray(aPosition);
            glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, false, 4 * sizeof(float), vertices + 2);
            glEnableVertexAttribArray(aTexCoord);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(uTexture, 1);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_BYTE, indices);
        }

        {
            auto path = tvg::Shape::gen();
            path->moveTo(199, 34);
            path->lineTo(253, 143);
            path->lineTo(374, 160);
            path->lineTo(287, 244);
            path->lineTo(307, 365);
            path->lineTo(199, 309);
            path->lineTo(97, 365);
            path->lineTo(112, 245);
            path->lineTo(26, 161);
            path->lineTo(146, 143);
            path->close();

            path->fill(150, 150, 255);

            path->stroke(3);
            path->stroke(0, 0, 255);
            path->stroke(tvg::StrokeJoin::Round);
            path->stroke(tvg::StrokeCap::Round);
            float pattern[2] = {10, 10};
            path->stroke(pattern, 2);

            tvg_canvas->push(std::move(path));

            tvg_canvas->draw();
            tvg_canvas->sync();
        }

        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport((fbWidth - tvg_canvas_width) / 2, (fbHeight - tvg_canvas_height) / 2, tvg_canvas_width,
                       tvg_canvas_height);

            glUseProgram(program);
            glVertexAttribPointer(aPosition, 2, GL_FLOAT, false, 4 * sizeof(float), vertices);
            glEnableVertexAttribArray(aPosition);
            glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, false, 4 * sizeof(float), vertices + 2);
            glEnableVertexAttribArray(aTexCoord);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tvg_texture);
            glUniform1i(uTexture, 1);
            glEnable(GL_BLEND);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_BYTE, indices);
            glDisable(GL_BLEND);
        }

        eglSwapBuffers(egl_display, egl_surface);
    }

    tvg::Initializer::term(tvg::CanvasEngine::Gl);

    glDeleteProgram(program);
    glDeleteFramebuffers(1, &tvg_framebuffer);
    glDeleteTextures(1, &tvg_texture);
    glDeleteTextures(1, &texture);

    destroyEGLSurface(egl_display, egl_surface);
    glfwDestroyWindow(window);
    glfwTerminate();
    destroyEGLContext(egl_display, egl_context);
    destroyEGLDisplay(egl_display);

    return 0;
}

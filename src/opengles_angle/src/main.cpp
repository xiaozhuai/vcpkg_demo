/**
 * Copyright 2022 xiaozhuai
 */

#define GLFW_INCLUDE_NONE

#include <cstdio>
#include <string>

#include "GLES3/gl3.h"
#include "GLFW/glfw3.h"
#include "egl_context.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    auto *window = glfwCreateWindow(960, 540, "vcpkg_demo_opengles_angle", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Could not create window\n");
        glfwTerminate();
        exit(1);
    }
    glfwSetKeyCallback(window, glfwKeyCallback);
    return window;
}

void initImgui(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
    colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 100");
}

void destroyImgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

GLuint createTextureFromFile(const std::string &file) {
    GLuint texture;
    glGenTextures(1, &texture);
    if (texture == 0) {
        fprintf(stderr, "Error create texture\n");
        exit(1);
    }
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t *pixels = stbi_load(file.c_str(), &width, &height, &channels, 4);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
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

    auto egl_display = initEglDisplay();
    auto egl_config = initEglConfig(egl_display);
    auto egl_context = initEglContext(egl_display, egl_config);
    auto egl_surface = createEglSurface(egl_display, egl_config, window);
    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    initImgui(window);

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

        if (showImgui) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, fbWidth, fbHeight);
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        eglSwapBuffers(egl_display, egl_surface);
    }

    glDeleteProgram(program);
    glDeleteTextures(1, &texture);

    destroyImgui();

    destroyEglSurface(egl_display, egl_surface);
    glfwDestroyWindow(window);
    glfwTerminate();
    destroyEglContext(egl_display, egl_context);
    destroyEglDisplay(egl_display);

    return 0;
}

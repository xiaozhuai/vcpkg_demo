//
// Copyright (c) 2026 xiaozhuai
//

#include "Renderer.h"

#include "RenderWindow.h"

namespace {

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

}  // namespace

Renderer::Renderer() = default;

Renderer::~Renderer() { stop(); }

void Renderer::start(RenderWindow *window) {
    if (rendering.exchange(true)) {
        return;
    }

    renderContext.init();
    surface = renderContext.createSurface(window);
    renderThread = std::thread([this]() {
        renderContext.makeCurrent(surface);

        const char *vert = R"(#version 300 es
layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

void main() {
    gl_Position = aPosition;
    vColor = aColor;
}
)";
        const char *frag = R"(#version 300 es
precision mediump float;
in vec3 vColor;
out vec4 fragColor;

void main() {
    fragColor = vec4(vColor, 1.0);
}
)";
        program = createProgram(vert, frag);
        while (rendering) {
            render();
        }
        glDeleteProgram(program);
        program = 0;
        renderContext.doneCurrent();
    });
}

void Renderer::stop() {
    rendering.store(false);
    if (renderThread.joinable()) {
        renderThread.join();
    }
    if (surface) {
        renderContext.destroySurface(surface);
        surface = nullptr;
    }
    renderContext.destroy();
}

void Renderer::render() {
    int width, height;
    renderContext.getSurfaceSize(surface, width, height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, width, height);
    const float vertices[] = {
        0.0f,  0.8f,  1.0f, 0.0f, 0.0f,  // top
        -0.8f, -0.8f, 0.0f, 1.0f, 0.0f,  // left
        0.8f,  -0.8f, 0.0f, 0.0f, 1.0f,  // right
    };
    glUseProgram(program);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), vertices + 2);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    renderContext.swapBuffers(surface);
}

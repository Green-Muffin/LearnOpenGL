//
// Created by GreenMuffin on 2025/12/29.
//

#include "renderer.h"

#include <iostream>

#include "shader.h"
#include "vertex_array.h"
#include "index_buffer.h"

void gl_clear_error() {
    while (glGetError() != GL_NO_ERROR);
}

bool gl_log_call(const char* function, const char* file, const int line) {
    while (const GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
                  " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() const {
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.bind();
    va.bind();
    ib.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}
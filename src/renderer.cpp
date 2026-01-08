//
// Created by GreenMuffin on 2025/12/29.
//

#include "renderer.h"

#include <iostream>

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
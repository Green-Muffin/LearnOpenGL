//
// Created by GreenMuffin on 2025/12/29.
//

#ifndef LEARNOPENGL_RENDERER_H
#define LEARNOPENGL_RENDERER_H

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __builtin_trap();

#define GLCall(x) gl_clear_error();\
x;\
ASSERT(gl_log_call(#x, __FILE__, __LINE__))

void gl_clear_error();

bool gl_log_call(const char* function, const char* file, int line);


#endif //LEARNOPENGL_RENDERER_H
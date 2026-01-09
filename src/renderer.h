//
// Created by GreenMuffin on 2025/12/29.
//

#ifndef LEARNOPENGL_RENDERER_H
#define LEARNOPENGL_RENDERER_H

#include <glad/glad.h>

#if defined(__APPLE__)
#define ASSERT(x) if (!(x)) __builtin_trap();
#elif defined(_WIN32)
#define ASSERT(x) if (!(x)) __debugbreak();
#else
#include <cassert>
#define ASSERT(x) assert(x)
#endif

#define GLCall(x) gl_clear_error();\
x;\
ASSERT(gl_log_call(#x, __FILE__, __LINE__))

class Shader;
class IndexBuffer;
class VertexArray;

void gl_clear_error();

bool gl_log_call(const char* function, const char* file, int line);

class Renderer {
public:

    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

private:

};


#endif //LEARNOPENGL_RENDERER_H
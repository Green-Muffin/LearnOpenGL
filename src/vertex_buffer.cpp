//
// Created by GreenMuffin on 2025/12/29.
//

#include "vertex_buffer.h"
#include "renderer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void *data, const unsigned int size){
    GLCall(glGenBuffers(1, &renderer_id_));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &renderer_id_);
}

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
}

void VertexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

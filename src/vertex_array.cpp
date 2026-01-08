//
// Created by GreenMuffin on 2026/1/8.
//

#include "vertex_array.h"

#include "renderer.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &renderer_id_));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &renderer_id_));
}

void VertexArray::add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    bind();
    vb.bind();
    const auto& elements = layout.get_elements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto&[type, count, normalized] = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, count, type,
                                     normalized, layout.get_stride(),
                                     reinterpret_cast<const void*>(offset)));
        offset += count * VertexBufferElement::get_size_of_type(type);
    }
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(renderer_id_));
}

void VertexArray::unbind() const {
    GLCall(glBindVertexArray(0));
}

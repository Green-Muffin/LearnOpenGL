//
// Created by GreenMuffin on 2026/1/8.
//

#ifndef LEARNOPENGL_VERTEX_ARRAY_H
#define LEARNOPENGL_VERTEX_ARRAY_H


class VertexBuffer;
class VertexBufferLayout;

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void bind() const;
    void unbind() const;

private:
    unsigned int renderer_id_{0};
};


#endif //LEARNOPENGL_VERTEX_ARRAY_H
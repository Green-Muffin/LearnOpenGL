//
// Created by GreenMuffin on 2025/12/29.
//

#ifndef LEARNOPENGL_VERTEX_BUFFER_H
#define LEARNOPENGL_VERTEX_BUFFER_H


class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
private:
    unsigned int renderer_id_ {0};
};


#endif //LEARNOPENGL_VERTEX_BUFFER_H
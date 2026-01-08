//
// Created by GreenMuffin on 2026/1/7.
//

#ifndef LEARNOPENGL_INDEX_BUFFER_H
#define LEARNOPENGL_INDEX_BUFFER_H


class IndexBuffer {
public:
    IndexBuffer(const void* data, unsigned int size);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
private:
    unsigned int renderer_id_ {0};
};


#endif //LEARNOPENGL_INDEX_BUFFER_H
//
// Created by GreenMuffin on 2026/1/7.
//

#ifndef LEARNOPENGL_INDEX_BUFFER_H
#define LEARNOPENGL_INDEX_BUFFER_H


class IndexBuffer {
public:
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    [[nodiscard]] unsigned int get_count() const { return count_; }

private:
    unsigned int renderer_id_ {0};
    unsigned int count_ {0};
};


#endif //LEARNOPENGL_INDEX_BUFFER_H
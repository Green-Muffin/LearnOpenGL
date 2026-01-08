//
// Created by GreenMuffin on 2026/1/8.
//

#ifndef LEARNOPENGL_VERTEX_BUFFER_LAYOUT_H
#define LEARNOPENGL_VERTEX_BUFFER_LAYOUT_H
#include <vector>

#include "glad/glad.h"


struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int get_size_of_type(const unsigned int type) {
        switch (type) {
            case GL_FLOAT: return sizeof(float);
            case GL_UNSIGNED_INT: return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
            default: ;
        }
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout(): stride_(0) {}

    template<typename T>
    void push(unsigned int count)
    {
        // 无条件触发编译期断言，导致编译失败（通常用于占位，表明该模板不应被直接实例化）
        static_assert(false);
    }

    template<>
    void push<float>(const unsigned int count)
    {
        elements_.push_back({ GL_FLOAT, count, GL_FALSE });
        stride_ += VertexBufferElement::get_size_of_type(GL_FLOAT) * count;
    }

    template<>
    void push<unsigned int>(const unsigned int count)
    {
        elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        stride_ += VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT) * count;
    }

    template<>
    void push<unsigned char>(const unsigned int count)
    {
        elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        stride_ += VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE) * count;
    }

    [[nodiscard]] std::vector<VertexBufferElement> get_elements() const { return elements_; }
    [[nodiscard]] unsigned int get_stride() const { return stride_; }

private:
    std::vector<VertexBufferElement> elements_;
    unsigned int stride_;
};

#endif //LEARNOPENGL_VERTEX_BUFFER_LAYOUT_H
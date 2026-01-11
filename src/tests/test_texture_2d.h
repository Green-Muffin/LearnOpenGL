//
// Created by GreenMuffin on 2026/1/11.
//

#ifndef LEARNOPENGL_TEST_TEXTURE_2D_H
#define LEARNOPENGL_TEST_TEXTURE_2D_H
#include <memory>

#include "index_buffer.h"
#include "shader.h"
#include "test.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"



namespace  tests {
    class TestTexture2D : public Test{
    public:
        TestTexture2D();
        ~TestTexture2D() override;

        void on_update(float delta_time) override;
        void on_render() override;
        void on_imgui_render() override;

    private:
        std::unique_ptr<VertexArray> _vao;
        std::unique_ptr<VertexBuffer> _vertex_buffer;
        std::unique_ptr<IndexBuffer> _index_buffer;
        std::unique_ptr<Shader> _shader;
        std::unique_ptr<Texture> _texture;

        glm::mat4 _projection, _view;
        glm::vec3 _translation_a, _translation_b;
    };
}


#endif //LEARNOPENGL_TEST_TEXTURE_2D_H
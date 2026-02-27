//
// Created by GreenMuffin(Redlive) on 2026/02/01.
//

#ifndef LEARNOPENGL_TEST_BATCH_RENDER_H
#define LEARNOPENGL_TEST_BATCH_RENDER_H
#include "test.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "index_buffer.h"
#include "redlive/redlive.h"
#include "shader.h"
#include "renderer.h"
#include "texture.h"


#include <memory>

namespace tests {

    class TestBatchRender : public Test {
    public:
        TestBatchRender();
        ~TestBatchRender() override;

        void on_update(float delta_time) override;
        void on_render() override;
        void on_imgui_render() override;

    private:
        scope<VertexArray> vao_;
        scope<VertexBuffer> vertex_buffer_;
		scope<IndexBuffer> index_buffer_;
        scope<Shader> shader_;
        scope<Texture> texture1_;
        scope<Texture> texture2_;

        Renderer renderer{};
    };
} // tests


#endif // LEARNOPENGL_TEST_BATCH_RENDER_H
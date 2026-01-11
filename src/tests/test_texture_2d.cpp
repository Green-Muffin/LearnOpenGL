//
// Created by GreenMuffin on 2026/1/11.
//

#include "test_texture_2d.h"

#include "imgui.h"
#include "renderer.h"
#include "vertex_buffer_layout.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace tests {
    TestTexture2D::TestTexture2D()
        :_projection(glm::ortho(0.0f, 960.0f, 0.0f, 720.0f, -1.0f, 1.0f)),
        _view(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
        _translation_a(glm::vec3(200, 200, 0)), _translation_b(glm::vec3(400, 200, 0)) {
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f, // 0
            200.0f, 100.0f, 1.0f, 0.0f,  // 1
            200.0f, 200.0f, 1.0f, 1.0f,    // 2
            100.0f, 200.0f, 0.0f, 1.0f   // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        _vao = std::make_unique<VertexArray>();

        _vertex_buffer = std::make_unique<VertexBuffer>(positions, 4 * 4);
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        _vao->add_buffer(*_vertex_buffer, layout);

        _index_buffer = std::make_unique<IndexBuffer>(indices, 6);

        _shader = std::make_unique<Shader>("../res/shader/basic.shader");
        _shader->bind();
        _shader->set_uniform_4f("u_color", 0.2f, 0.3f, 0.8f, 1.0f);
        _shader->set_uniform_1i("u_texture", 0);

        _texture = std::make_unique<Texture>("../res/textures/ChernoLogo.png");
	}

	TestTexture2D::~TestTexture2D() {
	}

	void TestTexture2D::on_update(float deltaTime) {
	}

	void TestTexture2D::on_render() {
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        _texture->bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), _translation_a);
            glm::mat4 mvp = _projection * _view * model;

            _shader->bind();
            _shader->set_uniform_mat4f("u_MVP", mvp);

            renderer.draw(*_vao, *_index_buffer, *_shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), _translation_b);
            glm::mat4 mvp = _projection * _view * model;

            _shader->bind();
            _shader->set_uniform_mat4f("u_MVP", mvp);

            renderer.draw(*_vao, *_index_buffer, *_shader);
        }
	}

	void TestTexture2D::on_imgui_render() {
        ImGui::SliderFloat3("_translation_a", &_translation_a.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("_translation_b", &_translation_b.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}

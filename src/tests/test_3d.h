//
// Created by GreenMuffin(Redlive) on 2026/01/29.
//

#ifndef LEARNOPENGL_TEST_3D_H
#define LEARNOPENGL_TEST_3D_H

#include "test.h"
#include "glm/glm.hpp"
#include "index_buffer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "renderer.h"

#include <memory>

namespace tests{

	class Test3D : public Test {
	public:
		Test3D();
		~Test3D() override;

		void on_start() override;
		void on_update(float delta_time) override;
		void on_render() override;
		void on_imgui_render() override;
		void on_end() override;

	private:
		glm::mat4 model_, view_, projection_;

		std::unique_ptr<VertexArray> vao_;
		std::unique_ptr<VertexBuffer> vertex_buffer_;
		std::unique_ptr<IndexBuffer> index_buffer_;
		std::unique_ptr<Shader> shader_;
		std::unique_ptr<Texture> texture_;

		Renderer renderer_{};
	};


} // tests

#endif // LEARNOPENGL_TEST_3D_H

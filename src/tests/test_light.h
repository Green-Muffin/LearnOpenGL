//
// Created by GreenMuffin(Redlive) on 2026/01/30.
//

#ifndef LEARNOPENGL_TEST_LIGHT_H
#define LEARNOPENGL_TEST_LIGHT_H

#include "test.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "index_buffer.h"
#include "redlive/redlive.h"
#include "shader.h"
#include "renderer.h"

namespace tests {

	class TestLight : public Test {
	public:
		TestLight();
		~TestLight() override;

		void on_start() override;

		void on_render() override;

	private:
		scope<VertexArray> obj_vao_, light_vao_;
		scope<IndexBuffer> index_buffer_;
		scope<VertexBuffer> vertex_buffer_;
		scope<Shader> obj_shader_, light_shader_;

		glm::mat4 obj_mvp_, light_mvp_;
		glm::mat4 obj_model_, light_model_, view_, projection_;

		Renderer renderer{};
	};

} // tests

#endif // LEARNOPENGL_TEST_LIGHT_H

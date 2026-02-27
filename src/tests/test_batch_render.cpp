//
// Created by GreenMuffin(Redlive) on 2026/02/01.
//

#include "test_batch_render.h"

namespace tests{

	TestBatchRender::TestBatchRender() {

		float vertices[] = {
			-1.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
			-1.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

			 0.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
			 1.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
			 1.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f
		};
		unsigned int indices[] = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7
		};

		vertex_buffer_ = create_scope<VertexBuffer>(vertices, sizeof(vertices));
		vao_ = create_scope<VertexArray>();
		index_buffer_ = create_scope<IndexBuffer>(indices, 12);

		VertexBufferLayout layout;
		layout.push<float>(3);	// TODO:make it a chain.
		layout.push<float>(4);
		layout.push<float>(2);
		layout.push<float>(1);

		vao_->add_buffer(*vertex_buffer_, layout);

		shader_ = create_scope<Shader>("batch.shader");
		shader_->set_uniform_1i("u_texture[0]", 0);
		shader_->set_uniform_1i("u_texture[1]", 1);

		texture1_ = create_scope<Texture>("grm.jpg");
		texture2_ = create_scope<Texture>("ChernoLogo.png");
		texture1_->bind(0);
		texture2_->bind(1);
	}

	TestBatchRender::~TestBatchRender() {

	}

	void TestBatchRender::on_update(const float delta_time) {

	}

	void TestBatchRender::on_render() {
		texture1_->bind(0);
		texture2_->bind(1);
		renderer.draw(*vao_, *index_buffer_, *shader_);
	}

	void TestBatchRender::on_imgui_render() {
		
	}

} // tests
//
// Created by GreenMuffin on 2026/3/8.
//

#include "Renderer.hpp"

#include "Application.hpp"

namespace LearnVulkan {
	void Renderer::draw_sprite() {
		DrawSpriteContext context{};
		context.pos = { -0.6f, -0.6f };
		context.size = { 1.2f, 1.2f };
		context.uv_rect = { 0.0f, 0.0f, 1.0f, 1.0f };
		context.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		context.texture_id = 0;
		draw_sprite(context);
	}

	void Renderer::draw_sprite(const DrawSpriteContext& context) {
		Application::self().renderSystem->render_resource->upload_sprite(context);
	}

	void Renderer::draw_model() {

	}
}
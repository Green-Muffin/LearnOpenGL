//
// Created by GreenMuffin on 2026/3/8.
//

#include "TestVulkanLayer.hpp"

#include "Graphics/Render/Renderer.hpp"
#include "Application.hpp"

namespace LearnVulkan {

	void TestVulkanLayer::onLogicTick(const float deltaTime) {
		// Position, Rotation, Scale, Color
		Renderer::draw_sprite();
	}

	void TestVulkanLayer::onRenderTick() {
		Application::self().renderSystem->draw();
	}
	
}
//
// Created by GreenMuffin on 2026/3/8.
//


#ifndef LEARN_TEST_VULKAN_LAYER_HPP
#define LEARN_TEST_VULKAN_LAYER_HPP

#include "Layer.hpp"

namespace LearnVulkan {
	class TestVulkanLayer : public Layer {
	public:
		TestVulkanLayer(const std::string& name) : Layer(name) {

		}

		~TestVulkanLayer() override = default;

		void onLogicTick(float deltaTime) override;
		void onRenderTick() override;

	private:

	};
}	

#endif // !LEARN_TEST_VULKAN_LAYER_HPP

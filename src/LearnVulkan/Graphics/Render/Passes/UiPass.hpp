//
// Created by GreenMuffin on 2026/2/27.
//

#ifndef LEARNVULKAN_UI_PASS_HPP
#define LEARNVULKAN_UI_PASS_HPP

#include "LearnVulkan/Graphics/interface/rhi.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <memory>

namespace LearnVulkan {

	class UiPass {
	public:
		UiPass() = default;
		~UiPass() = default;

		void initialize(gfx::RHI* rhi, GLFWwindow* window, gfx::RHIRenderPass* render_pass, uint32_t subpass = 0);
		void begin_frame();
		void end_frame(gfx::RHICommandBuffer* command_buffer);
		void shutdown();

	private:
		gfx::RHI* rhi_{ nullptr };
		GLFWwindow* window_{ nullptr };
		gfx::RHIRenderPass* render_pass_{ nullptr };
		uint32_t subpass_{ 0 };
		bool initialized_{ false };
	};

} // LearnVulkan


#endif//LEARNVULKAN_UI_PASS_HPP

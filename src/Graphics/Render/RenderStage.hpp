//
// Created by GreenMuffin on 2026/3/8.
//

#ifndef LEARNVULKAN_RENDERPASS_HPP
#define LEARNVULKAN_RENDERPASS_HPP

#include "Graphics/interface/rhi.h"
#include "RenderType.hpp"

#include <memory>

namespace LearnVulkan {

	struct RenderStageInitInfo {
		std::shared_ptr<gfx::RHI> rhi;
	};

	class RenderStage {
	public:
		struct FramebufferContext {
			int width{ 0 };
			int height{ 0 };
			gfx::RHIFramebuffer* framebuffer{ nullptr };
			gfx::RHIRenderPass* render_pass{ nullptr };
		};

		struct PipelineContext {
			gfx::RHIPipeline* pipeline{ nullptr };
			gfx::RHIPipelineLayout* layout{ nullptr };
		};

		void initialize(RenderStageInitInfo initInfo);
		void shutdown();

		void draw();
		void draw_forwad(const DrawResContext& context, uint32_t current_swapchain_image_index);

	private:
		std::shared_ptr<gfx::RHI> rhi_{ nullptr };

		FramebufferContext framebuffer_context_;
		PipelineContext pipeline_context_;
		gfx::RHIDescriptorSetLayout* texture_set_layout_{ nullptr };

		std::vector<gfx::RHIFramebuffer*> swapchain_framebuffers_;

		void update_quad_vertices(const DrawSpriteContext& sprite);
		void setup_render_pass();
		void setup_render_pipeline();
		void setup_framebuffers();
		void setup_descriptor_layouts();
	};
}


#endif // !LEARNVULKAN_RENDERPASS_HPP

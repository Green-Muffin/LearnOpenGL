//
// Created by GreenMuffin on 2026/3/4.
//

#include "RenderSystem.hpp"

#include "Graphics/interface/vulkan/vulkan_rhi.h"

namespace LearnVulkan {

	void RenderSystem::initialize(RenderSystemInitInfo init_info) {
		rhi_ = std::make_shared<gfx::VulkanRHI>();
		rhi_->initialize({ init_info.window, init_info.width, init_info.height });
		init_info_ = init_info;

		shader_library.initialize({ rhi_ });

		render_resource = std::make_shared<RenderResource>();
		render_resource->initialize({ rhi_ });

		render_graph_ = std::make_shared<RenderGraph>();
		render_graph_->initialize({ rhi_ });
		
    }

	void RenderSystem::shutdown() {
		if (rhi_) {
			rhi_->waitForFences();
			if (rhi_->getGraphicsQueue()) {
				rhi_->queueWaitIdle(rhi_->getGraphicsQueue());
			}
		}

		if (render_graph_) {
			render_graph_->shutdown();
			render_graph_.reset();
		}

		if (render_resource) {
			render_resource->shutdown();
			render_resource.reset();
		}

		shader_library.shutdown();
		rhi_.reset();
    }

	void RenderSystem::draw() {
		render();
	}

	void RenderSystem::render() {
		rhi_->prepareContext();

		render_graph_->forwad_render(render_resource->context);
		render_resource->context.sprites.clear();
	}
}

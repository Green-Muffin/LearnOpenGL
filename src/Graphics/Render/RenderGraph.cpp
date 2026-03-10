//
// Created by GreenMuffin on 2026/3/8.
//

#include "RenderGraph.hpp"

namespace LearnVulkan {

	void RenderGraph::initialize(RenderGraphInitInfo init_info) {
		rhi_ = init_info.rhi;

		main_stage_ = std::make_shared<RenderStage>();
		main_stage_->initialize({ rhi_ });
	}

	void RenderGraph::shutdown() {
		main_stage_->shutdown();
		main_stage_.reset();
	}

	void RenderGraph::draw() {
		main_stage_->draw();
	}

	void RenderGraph::forwad_render(const DrawResContext& context) {
		rhi_->waitForFences();
		if (!rhi_->prepareBeforePass([]() {})) { return; }

		main_stage_->draw_forwad(context, rhi_->getCurrentSwapchainImageIndex());
		rhi_->submitRendering([]() {});
	}

}
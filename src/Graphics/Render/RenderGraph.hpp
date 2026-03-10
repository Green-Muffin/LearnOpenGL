//
// Created by GreenMuffin on 2026/3/8.
//

#ifndef LEARNVULKAN_RENDER_GRAPH_HPP
#define LEARNVULKAN_RENDER_GRAPH_HPP

#include "Graphics/interface/rhi.h"
#include "RenderStage.hpp"

#include <memory>

namespace LearnVulkan {

	struct RenderGraphInitInfo {
		std::shared_ptr<gfx::RHI> rhi;
	};

	class RenderGraph {
	public:
		void initialize(RenderGraphInitInfo init_info);
		void shutdown();

		void draw();
		void forwad_render(const DrawResContext& context);
	private:
		std::shared_ptr<gfx::RHI> rhi_;

		std::shared_ptr<RenderStage> main_stage_;
	};

}

#endif // !LEARNVULKAN_RENDER_GRAPH_HPP

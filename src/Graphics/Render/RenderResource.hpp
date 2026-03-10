//
// Created by GreenMuffin on 2026/3/8.
//

#ifndef LEARNVULKAN_RENDER_RESOURCE_HPP
#define LEARNVULKAN_RENDER_RESOURCE_HPP

#include "RenderType.hpp"
#include "Graphics/interface/rhi.h"

#include "glm/glm.hpp"

#include <string>
#include <vector>

namespace LearnVulkan {

	struct RenderResourceInitInfo {
		std::shared_ptr<gfx::RHI> rhi;
	};

	class RenderResource {
	public:
		DrawResContext context;
		std::shared_ptr<QuadGpuData> quad;
		std::vector<TextureGpuData> textures;

		void initialize(RenderResourceInitInfo init_info);
		void shutdown();

		void upload_quad();
		void upload_sprite(const DrawSpriteContext& draw_context);
		uint32_t load_texture(const std::string& path);
		const TextureGpuData* get_texture(uint32_t texture_id) const;
		void update_perframe_buffer();

	private:
		std::shared_ptr<gfx::RHI> rhi_;

		void update_vertex_buffer();
		void update_index_buffer();
	};

}


#endif // !LEARNVULKAN_RENDER_RESOURCE_HPP

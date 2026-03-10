//
// Created by GreenMuffin on 2026/3/9.
//

#ifndef LEARNVULKAN_RENDER_TYPE_HPP
#define LEARNVULKAN_RENDER_TYPE_HPP

#include "Graphics/interface/rhi.h"
#include "glm/glm.hpp"

namespace LearnVulkan {

	struct Vertex {
		float px, py, pz;
		float u, v;
	};

	struct TextureData {
		uint32_t width{ 0 };
		uint32_t height{ 0 };
		uint32_t depth{ 0 };
		uint32_t mip_level{ 0 };
		uint32_t array_layers{ 0 };
		void* pixels{ nullptr };

		gfx::RHIFormat format{ gfx::RHI_FORMAT_MAX_ENUM };

		TextureData() = default;
		~TextureData() {
			if (pixels) {
				free(pixels);
			}
		}

		bool is_valid() const { return pixels != nullptr; }
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};

	struct MaterialData {
		void* base_color_pixels{ nullptr };
		uint32_t width{ 1 };
		uint32_t height{ 1 };
		gfx::RHIFormat format{ gfx::RHI_FORMAT_R8G8B8A8_SRGB };
	};

	struct Perframe {
		glm::mat4x4 view_proj;
	};

	struct QuadGpuData {
		gfx::RHIBuffer* vertex_buffer{ nullptr };
		gfx::RHIBuffer* index_buffer{ nullptr };
		gfx::RHIDeviceMemory* vertex_memory{ nullptr };
		gfx::RHIDeviceMemory* index_memory{ nullptr };
		VmaAllocation vertex_alloc{};
		VmaAllocation index_alloc{};
		uint32_t index_count{ 6 };
	};

	struct TextureGpuData {
		gfx::RHIImage* image{ nullptr };
		gfx::RHIImageView* view{ nullptr };
		gfx::RHISampler* sampler{ nullptr };
		VmaAllocation alloc{};
		gfx::RHIDescriptorSet* set{ nullptr };
	};

	struct DrawSpriteContext {
		glm::vec2 pos;
		glm::vec2 size;
		glm::vec4 uv_rect;
		glm::vec4 color;

		uint32_t texture_id;
	};


	struct DrawResContext {
		std::vector<DrawSpriteContext> sprites;
	};

}

#endif // !LEARNVULKAN_RENDER_TYPE_HPP;


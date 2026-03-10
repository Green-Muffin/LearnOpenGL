//
// Created by GreenMuffin on 2026/3/8.
//

#include "RenderResource.hpp"

#include "Graphics/interface/vulkan/vulkan_rhi.h"
#include "Graphics/interface/vulkan/vulkan_rhi_resource.h"

#include "stb_image.h"

#include <cstring>
#include <limits>

namespace LearnVulkan {

	void RenderResource::initialize(RenderResourceInitInfo init_info) {
		rhi_ = init_info.rhi;
		quad = std::make_shared<QuadGpuData>();

        upload_quad();
		load_texture("../../../res/textures/ChernoLogo.png");
	}

	void RenderResource::shutdown() {
		gfx::VulkanRHI* vulkan_context = static_cast<gfx::VulkanRHI*>(rhi_.get());

		for (auto& tex : textures) {
			if (tex.view) {
				rhi_->destroyImageView(tex.view);
                tex.view = nullptr;
			}
			if (tex.alloc != nullptr && tex.image != nullptr) {
				auto* vk_image = static_cast<gfx::VulkanImage*>(tex.image);
				vmaDestroyImage(vulkan_context->m_assets_allocator, vk_image->getResource(), tex.alloc);
			}
			if (tex.sampler) {
				tex.sampler = nullptr;
			}
		}
		textures.clear();

		if (quad) {
			if (quad->vertex_buffer) {
				rhi_->destroyBuffer(quad->vertex_buffer);
			}
			if (quad->index_buffer) {
				rhi_->destroyBuffer(quad->index_buffer);
			}
			if (quad->vertex_memory) {
				rhi_->freeMemory(quad->vertex_memory);
			}
			if (quad->index_memory) {
				rhi_->freeMemory(quad->index_memory);
			}
		}
		quad.reset();
		context.sprites.clear();

	}

	void RenderResource::upload_quad() {
        const Vertex vertices[4] = {
            {-0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
            { 0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
            { 0.5f,  0.5f, 0.0f, 1.0f, 0.0f},
            {-0.5f,  0.5f, 0.0f, 0.0f, 0.0f}
        };
        const uint16_t indices[6] = { 0, 1, 2, 2, 3, 0 };

        const gfx::RHIDeviceSize vb_size = sizeof(vertices);
        const gfx::RHIDeviceSize ib_size = sizeof(indices);

        rhi_->createBuffer(vb_size,
            gfx::RHI_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            gfx::RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT | gfx::RHI_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            quad->vertex_buffer,
            quad->vertex_memory);

        void* vb_ptr = nullptr;
        rhi_->mapMemory(quad->vertex_memory, 0, vb_size, 0, &vb_ptr);
        memcpy(vb_ptr, vertices, (size_t)vb_size);
        rhi_->unmapMemory(quad->vertex_memory);

        rhi_->createBuffer(ib_size,
            gfx::RHI_BUFFER_USAGE_INDEX_BUFFER_BIT,
            gfx::RHI_MEMORY_PROPERTY_HOST_VISIBLE_BIT | gfx::RHI_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            quad->index_buffer,
            quad->index_memory);

        void* ib_ptr = nullptr;
        rhi_->mapMemory(quad->index_memory, 0, ib_size, 0, &ib_ptr);
        memcpy(ib_ptr, indices, (size_t)ib_size);
        rhi_->unmapMemory(quad->index_memory);

        quad->index_count = 6;
	}

    void RenderResource::upload_sprite(const DrawSpriteContext& draw_context) {
        context.sprites.push_back(draw_context);
    }

    uint32_t RenderResource::load_texture(const std::string& path) {
        int width = 0;
        int height = 0;
        int channels = 0;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!pixels) {
            return std::numeric_limits<uint32_t>::max();
        }

        TextureGpuData tex{};
        tex.sampler = rhi_->getOrCreateDefaultSampler(gfx::Default_Sampler_Linear);
        rhi_->createGlobalImage(
            tex.image,
            tex.view,
            tex.alloc,
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height),
            pixels,
            gfx::RHI_FORMAT_R8G8B8A8_SRGB,
            1);

        stbi_image_free(pixels);
        textures.push_back(tex);
        return static_cast<uint32_t>(textures.size() - 1);
    }

    const TextureGpuData* RenderResource::get_texture(uint32_t texture_id) const {
        if (texture_id >= textures.size()) {
            return nullptr;
        }
        return &textures[texture_id];
    }

	void RenderResource::update_perframe_buffer() {

	}

	void RenderResource::update_vertex_buffer() {

	}

	void RenderResource::update_index_buffer() {

	}
}
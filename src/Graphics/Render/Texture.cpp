//
// Created by GreenMuffin on 2026/3/8.
//

#include "Texture.hpp"

#include "RenderType.hpp"

#include "stb_image.h"

#include <stdexcept>

namespace LearnVulkan {

	Texture::Texture(const std::string& path) {

	}

	void TextureLibrary::initialize(TextureLibraryInitInfo init_info) {
		rhi_ = init_info.rhi;
	}

	void TextureLibrary::shutdown() {

	}

	void TextureLibrary::load_texture(const std::string& path) {
		int w, h, channel;
		stbi_uc* pixels = stbi_load(path.c_str(), &w, &h, &channel, STBI_rgb_alpha);

		if (!pixels) { throw std::runtime_error("Image load failed!"); }

		TextureGpuData tex;

		rhi_->createGlobalImage(tex.image, tex.view, tex.alloc, static_cast<uint32_t>(w), static_cast<uint32_t>(h), pixels, gfx::RHI_FORMAT_R8G8B8A8_SRGB);
		stbi_image_free(pixels);
	}

}
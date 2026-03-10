//
// Created by GreenMuffin on 2026/3/8.
//

#ifndef LEARNVULKAN_TEXTURE_HPP
#define LEARNVULKAN_TEXTURE_HPP

#include "Graphics/interface/rhi.h"

#include <string>

namespace LearnVulkan {
	class Texture {
	public:
		explicit Texture(const std::string& path);

	private:
	};

	struct TextureLibraryInitInfo {
		std::shared_ptr<gfx::RHI> rhi;
	};

	class TextureLibrary {
	public:
		void initialize(TextureLibraryInitInfo init_info);
		void shutdown();

		void load_texture(const std::string& path);

	private:
		std::shared_ptr<gfx::RHI> rhi_;
	};
}


#endif // !LEARNVULKAN_TEXTURE_HPP

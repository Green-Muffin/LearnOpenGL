//
// Created by GreenMuffin on 2026/3/8.
//

#ifndef LEARNVULKAN_SHADER_HPP
#define LEARNVULKAN_SHADER_HPP

#include "Graphics/interface/rhi.h"

#include <string>
#include <memory>
#include <vector>

namespace gfx {
	class RHIShader;
}

namespace LearnVulkan {

	class Shader {
	public:
		Shader(const std::string& vert_path, const std::string& frag_path);
		~Shader();

		gfx::RHIShader* vert_shader_module();
		gfx::RHIShader* frag_shader_module();

	private:
		gfx::RHIShader* vert_shader_module_{ nullptr };
		gfx::RHIShader* frag_shader_module_{ nullptr };
	};

	struct ShaderLibraryInitInfo {
		std::shared_ptr<gfx::RHI> rhi;
	};

	class ShaderLibrary {
	public:
		std::shared_ptr<gfx::RHI> rhi{ nullptr };

		void initialize(ShaderLibraryInitInfo init_info);
		void shutdown();

		gfx::RHIShader* load_shader(const std::string& path);

	private:
		std::unordered_map<std::string, std::unique_ptr<gfx::RHIShader>> shader_umap_;
	};
}

#endif // !LEARNVULKAN_SHADER_HPP

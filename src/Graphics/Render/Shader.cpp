//
// Created by GreenMuffin on 2026/3/8.
//

#include "Shader.hpp"

#include "Graphics/interface/rhi.h"

#include <fstream>
#include <iostream>
#include <vector>

namespace utils {
	std::vector<unsigned char> read_whole_file(const std::string& path) {
		std::ifstream file(path, std::ios::binary | std::ios::ate);

		if (!file.is_open()) {
			std::cout << "Read " << path << " failed!\n";
			return std::vector<unsigned char>();
		}

		auto size = file.tellg();
		std::vector<char> content(size);
		file.seekg(0); 
		file.read(content.data(), content.size());

		std::vector<unsigned char> result(content.begin(), content.end());

		return result;
	}
}

namespace LearnVulkan {

	Shader::Shader(const std::string& vert_path, const std::string& frag_path) {
		const auto vert_shader = utils::read_whole_file(vert_path);
		const auto frag_shader = utils::read_whole_file(frag_path);
		//vert_shader_module_ = ShaderLibrary::self().rhi->createShaderModule(vert_shader);
		//frag_shader_module_ = ShaderLibrary::self().rhi->createShaderModule(frag_shader);
	}

	Shader::~Shader() {
		//ShaderLibrary::self().rhi->destroyShaderModule(vert_shader_module_);
		//ShaderLibrary::self().rhi->destroyShaderModule(frag_shader_module_);
	}

	gfx::RHIShader* Shader::vert_shader_module() {
		return vert_shader_module_;
	}

	gfx::RHIShader* Shader::frag_shader_module() {
		return frag_shader_module_;
	}

	void ShaderLibrary::initialize(ShaderLibraryInitInfo init_info) {
		rhi = init_info.rhi;
	}

	void ShaderLibrary::shutdown() {
		for (auto& [path, shader] : shader_umap_) {
			rhi->destroyShaderModule(shader.get());
		}
		shader_umap_.clear();
		std::unordered_map<std::string, std::unique_ptr<gfx::RHIShader>>().swap(shader_umap_);
	}

	gfx::RHIShader* ShaderLibrary::load_shader(const std::string& path) {
		if (shader_umap_.contains(path)) {
			return shader_umap_.at(path).get();
		}
		const auto shader = utils::read_whole_file(path);
		gfx::RHIShader* shader_module = rhi->createShaderModule(shader);
		shader_umap_.emplace(path, std::unique_ptr<gfx::RHIShader>(shader_module));
		return shader_umap_.at(path).get();
	}
}
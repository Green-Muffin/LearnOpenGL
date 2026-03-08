//
// Created by GreenMuffin on 2026/3/4.
//

#include "RenderSystem.hpp"

#include "Graphics/interface/vulkan/vulkan_rhi.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace LearnVulkan {

    namespace utils {
        std::vector<char> read_binary_files(const std::string& path) {
		namespace fs = std::filesystem;
		const fs::path requested(path);
		std::vector<fs::path> candidates;
		candidates.emplace_back(requested);
		candidates.emplace_back(fs::path("..") / requested);
		candidates.emplace_back(fs::path("..") / ".." / requested);
		candidates.emplace_back(fs::path("..") / ".." / ".." / requested);
		candidates.emplace_back(fs::path("..") / ".." / ".." / ".." / requested);
		candidates.emplace_back(fs::path("..") / ".." / ".." / ".." / ".." / requested);

		for (const auto& candidate : candidates) {
			if (!fs::exists(candidate)) {
				continue;
			}

			std::ifstream file(candidate, std::ios::ate | std::ios::binary);
			if (!file.is_open()) {
				continue;
			}

			size_t file_size = static_cast<size_t>(file.tellg());
			std::vector<char> buffer(file_size);
			file.seekg(0);
			file.read(buffer.data(), static_cast<std::streamsize>(file_size));
			file.close();
			return buffer;
		}

		std::ostringstream oss;
		oss << "failed to open shader file: " << path << "\n";
		oss << "current working directory: " << fs::current_path().string() << "\n";
		oss << "tried paths:";
		for (const auto& candidate : candidates) {
			oss << "\n  - " << candidate.lexically_normal().string();
		}

		throw std::runtime_error(oss.str());
	}
    } // utils

	void RenderSystem::initialize(GLFWwindow* native_window) {
		native_window_ = native_window;
		initializeRhi();
    }

	void RenderSystem::shutdown() {
		cleanupRhi();
    }

	void RenderSystem::drawFrame() {
		bool swapchain_recreated = false;
		auto recreate_resources = [this, &swapchain_recreated]() {
			recreateSwapchainResources();
			swapchain_recreated = true;
		};

		_rhi->waitForFences();
		if (!_rhi->prepareBeforePass(recreate_resources)) {
			return;
		}
		if (swapchain_recreated) {
			return;
		}
		_rhi->prepareContext();

		const uint32_t image_index = _rhi->getCurrentSwapchainImageIndex();
		if (image_index >= _framebuffers.size()) {
			return;
		}

		ui_pass_.begin_frame();

		gfx::RHIClearValue clear_values[1]{};
		clear_values[0].color.float32[0] = 0.07f;
		clear_values[0].color.float32[1] = 0.07f;
		clear_values[0].color.float32[2] = 0.10f;
		clear_values[0].color.float32[3] = 1.00f;

		const auto swapchain_info = _rhi->getSwapchainInfo();
		gfx::RHIRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = gfx::RHI_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = _renderPass;
		render_pass_info.framebuffer = _framebuffers[image_index];
		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = swapchain_info.extent;
		render_pass_info.clearValueCount = 1;
		render_pass_info.pClearValues = clear_values;

		gfx::RHICommandBuffer* command_buffer = _rhi->getCurrentCommandBuffer();
		_rhi->cmdBeginRenderPassPFN(command_buffer, &render_pass_info, gfx::RHI_SUBPASS_CONTENTS_INLINE);
		_rhi->cmdBindPipelinePFN(command_buffer, gfx::RHI_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
		_rhi->cmdDraw(command_buffer, 3, 1, 0, 0);
		ui_pass_.end_frame(command_buffer);
		_rhi->cmdEndRenderPassPFN(command_buffer);

		_rhi->submitRendering(recreate_resources);
	}

    void RenderSystem::initializeRhi() {
		_rhi = std::make_shared<gfx::VulkanRHI>();
		_rhi->initialize({ native_window_, window_width_, window_height_ });

		createRenderPass();
		createGraphicsPipeline();
		createFrambuffer();
    }

	void RenderSystem::createRenderPass() {
		// Create Pipeline Layout
		// Create Render Pass
			// Create Subpass
		
		const auto swapchainInfo = _rhi->getSwapchainInfo();

		gfx::RHIAttachmentDescription colorAttachment{};
		colorAttachment.format = swapchainInfo.imageFormat;							
		colorAttachment.samples = gfx::RHI_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = gfx::RHI_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = gfx::RHI_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = gfx::RHI_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = gfx::RHI_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = gfx::RHI_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = gfx::RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		gfx::RHIAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = gfx::RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		gfx::RHISubpassDescription subpass{};
		subpass.pipelineBindPoint = gfx::RHI_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		
		gfx::RHISubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;		// -----------
		dependency.dstSubpass = 0;
		dependency.srcStageMask = gfx::RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | gfx::RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = gfx::RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | gfx::RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = gfx::RHI_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | gfx::RHI_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		gfx::RHIRenderPassCreateInfo createInfo{};
		createInfo.sType = gfx::RHI_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &colorAttachment;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;

		if (!_rhi->createRenderPass(&createInfo, _renderPass) || !_renderPass) {
			throw std::runtime_error("failed to create render pass");
		}
	}
    
    void RenderSystem::createGraphicsPipeline() {
		// Create Graphics Pipeline
		// 1. Shader prepare
		auto vert_shader_code = utils::read_binary_files("res/shader/triangle.vert.spv");
		auto frag_shader_code = utils::read_binary_files("res/shader/triangle.frag.spv");

		std::vector<unsigned char> vert_bytes(vert_shader_code.begin(), vert_shader_code.end());
		std::vector<unsigned char> frag_bytes(frag_shader_code.begin(), frag_shader_code.end());

		gfx::RHIShader* vert_shader = _rhi->createShaderModule(vert_bytes);
		gfx::RHIShader* frag_shader = _rhi->createShaderModule(frag_bytes);
		if (!vert_shader || !frag_shader) {
			throw std::runtime_error("failed to create shader modules");
		}

		gfx::RHIPipelineShaderStageCreateInfo shader_stages[2]{};
		shader_stages[0].sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[0].stage = gfx::RHI_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = vert_shader;
		shader_stages[0].pName = "main";

		shader_stages[1].sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[1].stage = gfx::RHI_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = frag_shader;
		shader_stages[1].pName = "main";
		// 2. Vertex input
		gfx::RHIPipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		// 3. Vertex assembly
		gfx::RHIPipelineInputAssemblyStateCreateInfo input_assembly{};
		input_assembly.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly.topology = gfx::RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly.primitiveRestartEnable = RHI_FALSE;

		// 4. Viewport and scissor
		const auto swapchain_info = _rhi->getSwapchainInfo();
		gfx::RHIViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapchain_info.extent.width);
		viewport.height = static_cast<float>(swapchain_info.extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		gfx::RHIRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = swapchain_info.extent;

		gfx::RHIPipelineViewportStateCreateInfo viewport_state{};
		viewport_state.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state.viewportCount = 1;
		viewport_state.pViewports = &viewport;
		viewport_state.scissorCount = 1;
		viewport_state.pScissors = &scissor;

		// 5. Rasteraizer
		gfx::RHIPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = RHI_FALSE;
		rasterizer.rasterizerDiscardEnable = RHI_FALSE;
		rasterizer.polygonMode = gfx::RHI_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = gfx::RHI_CULL_MODE_NONE;
		rasterizer.frontFace = gfx::RHI_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = RHI_FALSE;

		// 6. Multisampler
		gfx::RHIPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = RHI_FALSE;
		multisampling.rasterizationSamples = gfx::RHI_SAMPLE_COUNT_1_BIT;

		// 7. Depth and stencil buffer
		gfx::RHIPipelineDepthStencilStateCreateInfo depth_stencil{};
		depth_stencil.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil.depthTestEnable = RHI_FALSE;
		depth_stencil.depthWriteEnable = RHI_FALSE;
		depth_stencil.depthCompareOp = gfx::RHI_COMPARE_OP_ALWAYS;
		depth_stencil.depthBoundsTestEnable = RHI_FALSE;
		depth_stencil.stencilTestEnable = RHI_FALSE;
		depth_stencil.minDepthBounds = 0.0f;
		depth_stencil.maxDepthBounds = 1.0f;

		// 8. Blending
		gfx::RHIPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.blendEnable = RHI_FALSE;
		color_blend_attachment.colorWriteMask = gfx::RHI_COLOR_COMPONENT_R_BIT |
			gfx::RHI_COLOR_COMPONENT_G_BIT |
			gfx::RHI_COLOR_COMPONENT_B_BIT |
			gfx::RHI_COLOR_COMPONENT_A_BIT;

		gfx::RHIPipelineColorBlendStateCreateInfo color_blending{};
		color_blending.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blending.logicOpEnable = RHI_FALSE;
		color_blending.logicOp = gfx::RHI_LOGIC_OP_COPY;
		color_blending.attachmentCount = 1;
		color_blending.pAttachments = &color_blend_attachment;

		// 9. Create Grapihics pipeline
		gfx::RHIPipelineDynamicStateCreateInfo dynamic_state{};
		dynamic_state.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state.dynamicStateCount = 0;
		dynamic_state.pDynamicStates = nullptr;

		gfx::RHIPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 0;
		pipeline_layout_info.pushConstantRangeCount = 0;
		if (!_rhi->createPipelineLayout(&pipeline_layout_info, _pipelineLayout) || _pipelineLayout == nullptr) {
			throw std::runtime_error("failed to create pipeline layout");
		}

		gfx::RHIGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType = gfx::RHI_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount = 2;
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_info;
		pipeline_info.pInputAssemblyState = &input_assembly;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterizer;
		pipeline_info.pMultisampleState = &multisampling;
		pipeline_info.pDepthStencilState = &depth_stencil;
		pipeline_info.pColorBlendState = &color_blending;
		pipeline_info.pDynamicState = &dynamic_state;
		pipeline_info.layout = _pipelineLayout;
		pipeline_info.renderPass = _renderPass;
		pipeline_info.subpass = 0;

		if (!_rhi->createGraphicsPipelines(nullptr, 1, &pipeline_info, _graphicsPipeline) || _graphicsPipeline == nullptr) {
			throw std::runtime_error("failed to create graphics pipeline");
		}

		_rhi->destroyShaderModule(frag_shader);
		_rhi->destroyShaderModule(vert_shader);
    }

	void RenderSystem::createFrambuffer() {
		const auto swapchain_info = _rhi->getSwapchainInfo();

		_framebuffers.resize(swapchain_info.imageViews.size(), nullptr);

		for (size_t i = 0; i < swapchain_info.imageViews.size(); i++) {
			gfx::RHIFramebufferCreateInfo framebuffer_info{};
			framebuffer_info.sType = gfx::RHI_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_info.renderPass = _renderPass;
			framebuffer_info.attachmentCount = 1;
			framebuffer_info.pAttachments = &swapchain_info.imageViews[i];
			framebuffer_info.width = swapchain_info.extent.width;
			framebuffer_info.height = swapchain_info.extent.height;
			framebuffer_info.layers = 1;

			if (!_rhi->createFramebuffer(&framebuffer_info, _framebuffers[i]) || _framebuffers[i] == nullptr) {
				throw std::runtime_error("failed to create framebuffer");
			}
		}
	}

	void RenderSystem::cleanupRhi() {
		if (!_rhi) {
			return;
		}

		if (_rhi->getGraphicsQueue()) {
			_rhi->queueWaitIdle(_rhi->getGraphicsQueue());
		}

		ui_pass_.shutdown();

		for (auto* framebuffer : _framebuffers) {
			if (framebuffer) {
				_rhi->destroyFramebuffer(framebuffer);
			}
		}
		_framebuffers.clear();

		if (_graphicsPipeline) {
			_rhi->destroyPipeline(_graphicsPipeline);
			_graphicsPipeline = nullptr;
		}
		if (_pipelineLayout) {
			_rhi->destroyPipelineLayout(_pipelineLayout);
			_pipelineLayout = nullptr;
		}
		if (_renderPass) {
			_rhi->destroyRenderPass(_renderPass);
			_renderPass = nullptr;
		}

		_rhi.reset();

	}

	void RenderSystem::recreateSwapchainResources() {
		for (auto* framebuffer : _framebuffers) {
			if (framebuffer) {
				_rhi->destroyFramebuffer(framebuffer);
			}
		}
		_framebuffers.clear();

		if (_graphicsPipeline) {
			_rhi->destroyPipeline(_graphicsPipeline);
			_graphicsPipeline = nullptr;
		}
		if (_pipelineLayout) {
			_rhi->destroyPipelineLayout(_pipelineLayout);
			_pipelineLayout = nullptr;
		}
		if (_renderPass) {
			_rhi->destroyRenderPass(_renderPass);
			_renderPass = nullptr;
		}

		createRenderPass();
		createGraphicsPipeline();
		createFrambuffer();
	}

}

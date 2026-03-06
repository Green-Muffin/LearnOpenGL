//
// Created by GreenMuffin on 2026/3/4.
//

#include "RenderSystem.hpp"

#include "LearnVulkan/Graphics/interface/vulkan/vulkan_rhi.h"

#include <filesystem>
#include <fstream>

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
		native_widnow_ = native_window;
		initialize_rhi();
    }

	void RenderSystem::shutdown() {
		cleanup_rhi();
    }

	void RenderSystem::initialize_rhi() {
        rhi_ = std::make_unique<gfx::VulkanRHI>();
        rhi_->initialize({native_widnow_, window_width_, window_height_});

        create_render_pass();
        create_graphics_pipeline();
        create_framebuffers();
		ui_pass_.initialize(rhi_.get(), native_widnow_, render_pass_, 0);
    }

    void RenderSystem::create_render_pass() {
		const auto swapchain_info = rhi_->getSwapchainInfo();
		const auto depth_info = rhi_->getDepthImageInfo();

		gfx::RHIAttachmentDescription color_attachment{};
		color_attachment.format = swapchain_info.image_format;
		color_attachment.samples = gfx::RHI_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp = gfx::RHI_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = gfx::RHI_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = gfx::RHI_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = gfx::RHI_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = gfx::RHI_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = gfx::RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		gfx::RHIAttachmentDescription depth_attachment{};
		depth_attachment.format = depth_info.depth_image_format;
		depth_attachment.samples = gfx::RHI_SAMPLE_COUNT_1_BIT;
		depth_attachment.loadOp = gfx::RHI_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.storeOp = gfx::RHI_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.stencilLoadOp = gfx::RHI_ATTACHMENT_LOAD_OP_DONT_CARE;
		depth_attachment.stencilStoreOp = gfx::RHI_ATTACHMENT_STORE_OP_DONT_CARE;
		depth_attachment.initialLayout = gfx::RHI_IMAGE_LAYOUT_UNDEFINED;
		depth_attachment.finalLayout = gfx::RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		gfx::RHIAttachmentReference color_attachment_ref{};
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout = gfx::RHI_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		gfx::RHIAttachmentReference depth_attachment_ref{};
		depth_attachment_ref.attachment = 1;
		depth_attachment_ref.layout = gfx::RHI_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		gfx::RHISubpassDescription subpass{};
		subpass.pipelineBindPoint = gfx::RHI_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment_ref;
		subpass.pDepthStencilAttachment = &depth_attachment_ref;

		gfx::RHISubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = gfx::RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | gfx::RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = gfx::RHI_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | gfx::RHI_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = gfx::RHI_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | gfx::RHI_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<gfx::RHIAttachmentDescription, 2> attachments = { color_attachment, depth_attachment };
		gfx::RHIRenderPassCreateInfo render_pass_info{};
		render_pass_info.sType = gfx::RHI_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = static_cast<uint32_t>(attachments.size());
		render_pass_info.pAttachments = attachments.data();
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpass;
		render_pass_info.dependencyCount = 1;
		render_pass_info.pDependencies = &dependency;

		if (!rhi_->createRenderPass(&render_pass_info, render_pass_) || render_pass_ == nullptr) {
			throw std::runtime_error("failed to create render pass");
		}
	
    }

    void RenderSystem::create_graphics_pipeline() {
		auto vert_shader_code = utils::read_binary_files("res/shader/triangle.vert.spv");
		auto frag_shader_code = utils::read_binary_files("res/shader/triangle.frag.spv");

		std::vector<unsigned char> vert_bytes(vert_shader_code.begin(), vert_shader_code.end());
		std::vector<unsigned char> frag_bytes(frag_shader_code.begin(), frag_shader_code.end());

		gfx::RHIShader* vert_shader = rhi_->createShaderModule(vert_bytes);
		gfx::RHIShader* frag_shader = rhi_->createShaderModule(frag_bytes);
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

		gfx::RHIPipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		gfx::RHIPipelineInputAssemblyStateCreateInfo input_assembly{};
		input_assembly.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly.topology = gfx::RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly.primitiveRestartEnable = RHI_FALSE;

		const auto swapchain_info = rhi_->getSwapchainInfo();
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

		gfx::RHIPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = RHI_FALSE;
		rasterizer.rasterizerDiscardEnable = RHI_FALSE;
		rasterizer.polygonMode = gfx::RHI_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = gfx::RHI_CULL_MODE_NONE;
		rasterizer.frontFace = gfx::RHI_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = RHI_FALSE;

		gfx::RHIPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = RHI_FALSE;
		multisampling.rasterizationSamples = gfx::RHI_SAMPLE_COUNT_1_BIT;

		gfx::RHIPipelineDepthStencilStateCreateInfo depth_stencil{};
		depth_stencil.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil.depthTestEnable = RHI_TRUE;
		depth_stencil.depthWriteEnable = RHI_TRUE;
		depth_stencil.depthCompareOp = gfx::RHI_COMPARE_OP_LESS;
		depth_stencil.depthBoundsTestEnable = RHI_FALSE;
		depth_stencil.stencilTestEnable = RHI_FALSE;

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

		gfx::RHIPipelineDynamicStateCreateInfo dynamic_state{};
		dynamic_state.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state.dynamicStateCount = 0;
		dynamic_state.pDynamicStates = nullptr;

		gfx::RHIPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 0;
		pipeline_layout_info.pushConstantRangeCount = 0;
		if (!rhi_->createPipelineLayout(&pipeline_layout_info, pipeline_layout_) || pipeline_layout_ == nullptr) {
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
		pipeline_info.layout = pipeline_layout_;
		pipeline_info.renderPass = render_pass_;
		pipeline_info.subpass = 0;

		if (!rhi_->createGraphicsPipelines(nullptr, 1, &pipeline_info, graphics_pipeline_) || graphics_pipeline_ == nullptr) {
			throw std::runtime_error("failed to create graphics pipeline");
		}

		rhi_->destroyShaderModule(frag_shader);
		rhi_->destroyShaderModule(vert_shader);
	
    }

    void RenderSystem::create_framebuffers() {
		const auto swapchain_info = rhi_->getSwapchainInfo();
		const auto depth_info = rhi_->getDepthImageInfo();

		swapchain_framebuffers_.resize(swapchain_info.imageViews.size(), nullptr);

		for (size_t i = 0; i < swapchain_info.imageViews.size(); i++) {
			gfx::RHIImageView* attachments[2] = {
				swapchain_info.imageViews[i],
				depth_info.depth_image_view
			};

			gfx::RHIFramebufferCreateInfo framebuffer_info{};
			framebuffer_info.sType = gfx::RHI_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebuffer_info.renderPass = render_pass_;
			framebuffer_info.attachmentCount = 2;
			framebuffer_info.pAttachments = attachments;
			framebuffer_info.width = swapchain_info.extent.width;
			framebuffer_info.height = swapchain_info.extent.height;
			framebuffer_info.layers = 1;

			if (!rhi_->createFramebuffer(&framebuffer_info, swapchain_framebuffers_[i]) || swapchain_framebuffers_[i] == nullptr) {
				throw std::runtime_error("failed to create framebuffer");
			}
		}
	
    }

    void RenderSystem::draw_frame() {
		rhi_->waitForFences();
		if (!rhi_->prepareBeforePass([]() {})) {
			return;
		}
		rhi_->prepareContext();

		const uint32_t image_index = rhi_->getCurrentSwapchainImageIndex();
		if (image_index >= swapchain_framebuffers_.size()) {
			return;
		}

		ui_pass_.begin_frame();

		gfx::RHIClearValue clear_values[2]{};
		clear_values[0].color.float32[0] = 0.07f;
		clear_values[0].color.float32[1] = 0.07f;
		clear_values[0].color.float32[2] = 0.10f;
		clear_values[0].color.float32[3] = 1.00f;
		clear_values[1].depthStencil.depth = 1.0f;
		clear_values[1].depthStencil.stencil = 0;

		const auto swapchain_info = rhi_->getSwapchainInfo();
		gfx::RHIRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = gfx::RHI_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = render_pass_;
		render_pass_info.framebuffer = swapchain_framebuffers_[image_index];
		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = swapchain_info.extent;
		render_pass_info.clearValueCount = 2;
		render_pass_info.pClearValues = clear_values;

		gfx::RHICommandBuffer* command_buffer = rhi_->getCurrentCommandBuffer();
		rhi_->cmdBeginRenderPassPFN(command_buffer, &render_pass_info, gfx::RHI_SUBPASS_CONTENTS_INLINE);
		rhi_->cmdBindPipelinePFN(command_buffer, gfx::RHI_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline_);
		rhi_->cmdDraw(command_buffer, 3, 1, 0, 0);
		ui_pass_.end_frame(command_buffer);
		rhi_->cmdEndRenderPassPFN(command_buffer);

		rhi_->submitRendering([]() {});
	
    }


    void RenderSystem::cleanup_rhi() {
		if (!rhi_) {
			return;
		}

		if (rhi_->getGraphicsQueue()) {
			rhi_->queueWaitIdle(rhi_->getGraphicsQueue());
		}

		ui_pass_.shutdown();

		for (auto* framebuffer : swapchain_framebuffers_) {
			if (framebuffer) {
				rhi_->destroyFramebuffer(framebuffer);
			}
		}
		swapchain_framebuffers_.clear();

		if (graphics_pipeline_) {
			rhi_->destroyPipeline(graphics_pipeline_);
			graphics_pipeline_ = nullptr;
		}
		if (pipeline_layout_) {
			rhi_->destroyPipelineLayout(pipeline_layout_);
			pipeline_layout_ = nullptr;
		}
		if (render_pass_) {
			rhi_->destroyRenderPass(render_pass_);
			render_pass_ = nullptr;
		}

		rhi_.reset();
	
    }
}
//
// Created by GreenMuffin on 2026/3/8.
//

#include "RenderStage.hpp"

#include "Shader.hpp"

#include "Application.hpp"

#include <stdexcept>
#include <array>
#include <cstddef>
#include <cstring>

namespace LearnVulkan {

	void RenderStage::initialize(RenderStageInitInfo initInfo) {
		rhi_ = initInfo.rhi;

		setup_render_pass();
		setup_descriptor_layouts();
		setup_render_pipeline();
		setup_framebuffers();
	}

	void RenderStage::shutdown() {
		for (auto framebuffer : swapchain_framebuffers_) {
			if (framebuffer) {
				rhi_->destroyFramebuffer(framebuffer);
			}
		}

		swapchain_framebuffers_.clear();
		std::vector<gfx::RHIFramebuffer*>().swap(swapchain_framebuffers_);

		if (pipeline_context_.pipeline) {
			rhi_->destroyPipeline(pipeline_context_.pipeline);
			pipeline_context_.pipeline = nullptr;
		}
		if (pipeline_context_.layout) {
			rhi_->destroyPipelineLayout(pipeline_context_.layout);
			pipeline_context_.layout = nullptr;
		}
		if (texture_set_layout_) {
			texture_set_layout_ = nullptr;
		}
		if (framebuffer_context_.render_pass) {
			rhi_->destroyRenderPass(framebuffer_context_.render_pass);
			framebuffer_context_.render_pass = nullptr;
		}
	}

	void RenderStage::draw() {
		draw_forwad(Application::self().renderSystem->render_resource->context, rhi_->getCurrentSwapchainImageIndex());
	}

	void RenderStage::draw_forwad(const DrawResContext& context, uint32_t current_swapchain_image_index) {
		const auto& render_resource = Application::self().renderSystem->render_resource;
		gfx::RHICommandBuffer* cmd = rhi_->getCurrentCommandBuffer();
		if (!render_resource || !render_resource->quad || current_swapchain_image_index >= swapchain_framebuffers_.size()) {
			return;
		}

		gfx::RHIClearValue clear_values[2]{};
		clear_values[0].color.float32[0] = 0.07f;
		clear_values[0].color.float32[1] = 0.07f;
		clear_values[0].color.float32[2] = 0.10f;
		clear_values[0].color.float32[3] = 1.00f;
		clear_values[1].depthStencil.depth = 1.0f;
		clear_values[1].depthStencil.stencil = 0;

		gfx::RHIRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = gfx::RHI_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = framebuffer_context_.render_pass;
		render_pass_info.framebuffer = swapchain_framebuffers_[current_swapchain_image_index];
		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = rhi_->getSwapchainInfo().extent;
		render_pass_info.clearValueCount = 2;
		render_pass_info.pClearValues = clear_values;

		rhi_->cmdBeginRenderPassPFN(cmd, &render_pass_info, gfx::RHI_SUBPASS_CONTENTS_INLINE);

		rhi_->cmdBindPipelinePFN(cmd, gfx::RHI_PIPELINE_BIND_POINT_GRAPHICS, pipeline_context_.pipeline);
		gfx::RHIBuffer* vbs[] = { render_resource->quad->vertex_buffer };
		gfx::RHIDeviceSize offs[] = { 0 };
		rhi_->cmdBindVertexBuffersPFN(cmd, 0, 1, vbs, offs);
		rhi_->cmdBindIndexBufferPFN(cmd, render_resource->quad->index_buffer, 0, gfx::RHI_INDEX_TYPE_UINT16);

		for (const auto& s : context.sprites) {
			update_quad_vertices(s);

			const auto* tex = render_resource->get_texture(s.texture_id);
			if (!tex || !tex->set) {
				continue;
			}
			gfx::RHIDescriptorSet* sets[] = { tex->set };
			rhi_->cmdBindDescriptorSetsPFN(
				cmd,
				gfx::RHI_PIPELINE_BIND_POINT_GRAPHICS,
				pipeline_context_.layout,
				0,
				1,
				sets,
				0,
				nullptr);
			rhi_->cmdDrawIndexedPFN(cmd, render_resource->quad->index_count, 1, 0, 0, 0);
		}

		rhi_->cmdEndRenderPassPFN(cmd);
	}

	void RenderStage::update_quad_vertices(const DrawSpriteContext& sprite) {
		const auto& render_resource = Application::self().renderSystem->render_resource;
		if (!render_resource || !render_resource->quad || !render_resource->quad->vertex_memory) {
			return;
		}

		const float left = sprite.pos.x;
		const float bottom = sprite.pos.y;
		const float right = sprite.pos.x + sprite.size.x;
		const float top = sprite.pos.y + sprite.size.y;

		const float u0 = sprite.uv_rect.x;
		const float v0 = sprite.uv_rect.y;
		const float u1 = sprite.uv_rect.z;
		const float v1 = sprite.uv_rect.w;

		Vertex vertices[4] = {
			{left,  bottom, 0.0f, u0, v1},
			{right, bottom, 0.0f, u1, v1},
			{right, top,    0.0f, u1, v0},
			{left,  top,    0.0f, u0, v0},
		};

		void* mapped = nullptr;
		rhi_->mapMemory(render_resource->quad->vertex_memory, 0, sizeof(vertices), 0, &mapped);
		memcpy(mapped, vertices, sizeof(vertices));
		rhi_->unmapMemory(render_resource->quad->vertex_memory);
	}

	void RenderStage::setup_render_pass() {
		const auto swapchain_info = rhi_->getSwapchainInfo();
		const auto depth_info = rhi_->getDepthImageInfo();

		gfx::RHIAttachmentDescription color_attachment{};
		color_attachment.format = swapchain_info.imageFormat;
		color_attachment.samples = gfx::RHI_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp = gfx::RHI_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp = gfx::RHI_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp = gfx::RHI_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = gfx::RHI_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout = gfx::RHI_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout = gfx::RHI_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		gfx::RHIAttachmentDescription depth_attachment{};
		depth_attachment.format = depth_info.depthImageFormat;
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
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;		// ???
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

		if (!rhi_->createRenderPass(&render_pass_info, framebuffer_context_.render_pass) || framebuffer_context_.render_pass == nullptr) {
			throw std::runtime_error("failed to create render pass");
		}
	}

	void RenderStage::setup_render_pipeline() {
		const auto vert_shader_module = Application::self().renderSystem->shader_library.load_shader("../../../res/shader/sprite.vert.spv");
		const auto frag_shader_module = Application::self().renderSystem->shader_library.load_shader("../../../res/shader/sprite.frag.spv");
		gfx::RHIPipelineShaderStageCreateInfo shader_stages[2]{};
		shader_stages[0].sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[0].stage = gfx::RHI_SHADER_STAGE_VERTEX_BIT;
		shader_stages[0].module = vert_shader_module;
		shader_stages[0].pName = "main";

		shader_stages[1].sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stages[1].stage = gfx::RHI_SHADER_STAGE_FRAGMENT_BIT;
		shader_stages[1].module = frag_shader_module;
		shader_stages[1].pName = "main";

		gfx::RHIPipelineVertexInputStateCreateInfo vertex_input_info{};
		vertex_input_info.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		gfx::RHIVertexInputBindingDescription vertex_binding{};
		vertex_binding.binding = 0;
		vertex_binding.stride = sizeof(Vertex);
		vertex_binding.inputRate = gfx::RHI_VERTEX_INPUT_RATE_VERTEX;

		std::array<gfx::RHIVertexInputAttributeDescription, 2> vertex_attributes{};
		vertex_attributes[0].location = 0;
		vertex_attributes[0].binding = 0;
		vertex_attributes[0].format = gfx::RHI_FORMAT_R32G32B32_SFLOAT;
		vertex_attributes[0].offset = offsetof(Vertex, px);
		vertex_attributes[1].location = 1;
		vertex_attributes[1].binding = 0;
		vertex_attributes[1].format = gfx::RHI_FORMAT_R32G32_SFLOAT;
		vertex_attributes[1].offset = offsetof(Vertex, u);

		vertex_input_info.vertexBindingDescriptionCount = 1;
		vertex_input_info.pVertexBindingDescriptions = &vertex_binding;
		vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_attributes.size());
		vertex_input_info.pVertexAttributeDescriptions = vertex_attributes.data();

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
		depth_stencil.depthTestEnable = RHI_FALSE;
		depth_stencil.depthWriteEnable = RHI_FALSE;
		depth_stencil.depthCompareOp = gfx::RHI_COMPARE_OP_ALWAYS;
		depth_stencil.depthBoundsTestEnable = RHI_FALSE;
		depth_stencil.stencilTestEnable = RHI_FALSE;

		gfx::RHIPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.blendEnable = RHI_TRUE;
		color_blend_attachment.srcColorBlendFactor = gfx::RHI_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment.dstColorBlendFactor = gfx::RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment.colorBlendOp = gfx::RHI_BLEND_OP_ADD;
		color_blend_attachment.srcAlphaBlendFactor = gfx::RHI_BLEND_FACTOR_ONE;
		color_blend_attachment.dstAlphaBlendFactor = gfx::RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment.alphaBlendOp = gfx::RHI_BLEND_OP_ADD;
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

		gfx::RHIDescriptorSetLayout* set_layouts[] = { texture_set_layout_ };
		gfx::RHIPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = gfx::RHI_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 1;
		pipeline_layout_info.pSetLayouts = set_layouts;
		pipeline_layout_info.pushConstantRangeCount = 0;
		if (!rhi_->createPipelineLayout(&pipeline_layout_info, pipeline_context_.layout) || pipeline_context_.layout == nullptr) {
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
		pipeline_info.layout = pipeline_context_.layout;
		pipeline_info.renderPass = framebuffer_context_.render_pass;
		pipeline_info.subpass = 0;

		if (!rhi_->createGraphicsPipelines(nullptr, 1, &pipeline_info, pipeline_context_.pipeline) || pipeline_context_.pipeline == nullptr) {
			throw std::runtime_error("failed to create graphics pipeline");
		}

		//rhi_->destroyShaderModule(frag_shader);
		//rhi_->destroyShaderModule(vert_shader);
	}

	void RenderStage::setup_framebuffers() {
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
			framebuffer_info.renderPass = framebuffer_context_.render_pass;
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

	void RenderStage::setup_descriptor_layouts() {
		gfx::RHIDescriptorSetLayoutBinding texture_binding{};
		texture_binding.binding = 0;
		texture_binding.descriptorType = gfx::RHI_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		texture_binding.descriptorCount = 1;
		texture_binding.stageFlags = gfx::RHI_SHADER_STAGE_FRAGMENT_BIT;

		gfx::RHIDescriptorSetLayoutCreateInfo layout_info{};
		layout_info.sType = gfx::RHI_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = 1;
		layout_info.pBindings = &texture_binding;
		if (!rhi_->createDescriptorSetLayout(&layout_info, texture_set_layout_) || texture_set_layout_ == nullptr) {
			throw std::runtime_error("failed to create texture descriptor set layout");
		}

		auto render_resource = Application::self().renderSystem->render_resource;
		for (auto& tex : render_resource->textures) {
			gfx::RHIDescriptorSetAllocateInfo alloc_info{};
			alloc_info.sType = gfx::RHI_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info.descriptorPool = rhi_->getDescriptorPoor();
			alloc_info.descriptorSetCount = 1;
			const gfx::RHIDescriptorSetLayout* set_layouts[] = { texture_set_layout_ };
			alloc_info.pSetLayouts = set_layouts;
			if (!rhi_->allocateDescriptorSets(&alloc_info, tex.set) || tex.set == nullptr) {
				throw std::runtime_error("failed to allocate texture descriptor set");
			}

			gfx::RHIDescriptorImageInfo image_info{};
			image_info.sampler = tex.sampler;
			image_info.imageView = tex.view;
			image_info.imageLayout = gfx::RHI_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			gfx::RHIWriteDescriptorSet write{};
			write.sType = gfx::RHI_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet = tex.set;
			write.dstBinding = 0;
			write.descriptorCount = 1;
			write.descriptorType = gfx::RHI_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.pImageInfo = &image_info;
			rhi_->updateDescriptorSets(1, &write, 0, nullptr);
		}
	}
}
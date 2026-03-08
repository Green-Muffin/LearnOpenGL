//
// Created by GreenMuffin on 2026/3/4.
//

#include "UiPass.hpp"

#include "LearnVulkan/Graphics/interface/vulkan/vulkan_rhi.h"

#include <stdexcept>


#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace LearnVulkan {

    void UiPass::initialize(gfx::RHI* rhi, GLFWwindow* window, gfx::RHIRenderPass* render_pass, uint32_t subpass) {
        if (initialized_) {
            return;
        }

        _rhi = rhi;
        window_ = window;
        render_pass_ = render_pass;
        subpass_ = subpass;

        if (_rhi == nullptr || window_ == nullptr || render_pass_ == nullptr) {
            throw std::runtime_error("UiPass initialize failed: invalid input resources");
        }

        auto* vulkan_rhi = dynamic_cast<gfx::VulkanRHI*>(_rhi);
        auto* vulkan_render_pass = static_cast<gfx::VulkanRenderPass*>(render_pass_);
        if (vulkan_rhi == nullptr || vulkan_render_pass == nullptr) {
            throw std::runtime_error("UiPass requires VulkanRHI and VulkanRenderPass");
        }

        ImGui_ImplGlfw_InitForVulkan(window_, true);

        ImGui_ImplVulkan_InitInfo init_info{};
        init_info.Instance = vulkan_rhi->m_instance;
        init_info.PhysicalDevice = vulkan_rhi->m_physical_device;
        init_info.Device = vulkan_rhi->m_device;
        init_info.QueueFamily = _rhi->getQueueFamilyIndices().graphics_family.value();
        init_info.Queue = static_cast<gfx::VulkanQueue*>(_rhi->getGraphicsQueue())->getResource();
        init_info.DescriptorPool = vulkan_rhi->m_vk_descriptor_pool;
        init_info.MinImageCount = static_cast<uint32_t>(gfx::VulkanRHI::k_max_frames_in_flight);
        init_info.ImageCount = static_cast<uint32_t>(gfx::VulkanRHI::k_max_frames_in_flight);
        init_info.PipelineInfoMain.RenderPass = vulkan_render_pass->getResource();
        init_info.PipelineInfoMain.Subpass = subpass_;
        init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        if (!ImGui_ImplVulkan_Init(&init_info)) {
            throw std::runtime_error("ImGui_ImplVulkan_Init failed");
        }

        initialized_ = true;
	}

    void UiPass::begin_frame() {
        if (!initialized_) {
            return;
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Vulkan ImGui");
        ImGui::Text("ImGui backend is running.");
        ImGui::Text("Frame index: %u", static_cast<uint32_t>(_rhi->getCurrentFrameIndex()));
        ImGui::End();

        static bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
    }

    void UiPass::end_frame(gfx::RHICommandBuffer* command_buffer) {
        if (!initialized_ || command_buffer == nullptr) {
            return;
        }

        ImGui_ImplVulkan_RenderDrawData(
            ImGui::GetDrawData(),
            static_cast<gfx::VulkanCommandBuffer*>(command_buffer)->getResource());
    }

    void UiPass::shutdown() {
        if (!initialized_) {
            return;
        }

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        initialized_ = false;
        _rhi = nullptr;
        window_ = nullptr;
        render_pass_ = nullptr;

	}

} // LearnVulkan
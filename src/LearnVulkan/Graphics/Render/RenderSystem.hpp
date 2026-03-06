//
// Created by GreenMuffin on 2025/3/4.
//

#ifndef LEARNVULKAN_RENDER_SYSTEM_HPP
#define LEARNVULKAN_RENDER_SYSTEM_HPP

#include "LearnVulkan/Graphics/interface/rhi.h"
#include "LearnVulkan/Graphics/Render/Passes/UiPass.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>

namespace LearnVulkan {

    class RenderSystem {
        friend class Application;
    public:
        RenderSystem() = default;
        ~RenderSystem() = default;

        void initialize(GLFWwindow* native_window);
        void shutdown();        
        void draw_frame();
    private:
        GLFWwindow* native_widnow_{ nullptr };
        std::unique_ptr<gfx::RHI> rhi_;

        gfx::RHIRenderPass* render_pass_{nullptr};
        gfx::RHIPipeline* graphics_pipeline_{nullptr};
        gfx::RHIPipelineLayout* pipeline_layout_{nullptr};
        std::vector<gfx::RHIFramebuffer*> swapchain_framebuffers_{nullptr};
        UiPass ui_pass_;

        uint32_t window_height_{1200};
        uint32_t window_width_{980};

        void initialize_rhi();
        void create_render_pass();
        void create_graphics_pipeline();
        void create_framebuffers();

        void cleanup_rhi();
    };

} // LearnVulkan


#endif//LEARNVULKAN_RENDER_SYSTEM_HPP
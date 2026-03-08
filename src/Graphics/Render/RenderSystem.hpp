//
// Created by GreenMuffin on 2025/3/4.
//

#ifndef LEARNVULKAN_RENDER_SYSTEM_HPP
#define LEARNVULKAN_RENDER_SYSTEM_HPP

#include "Graphics/interface/rhi.h"
#include "Graphics/Render/Passes/UiPass.hpp"

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
        void drawFrame();
    private:
        GLFWwindow* native_window_{ nullptr };
        UiPass ui_pass_;

        uint32_t window_height_{1200};
        uint32_t window_width_{980};
        std::shared_ptr<gfx::RHI> _rhi{ nullptr };

        gfx::RHIRenderPass* _renderPass{ nullptr };
        gfx::RHIPipelineLayout* _pipelineLayout{ nullptr };
        gfx::RHIPipeline* _graphicsPipeline{ nullptr };
        std::vector<gfx::RHIFramebuffer*> _framebuffers{};

        void initializeRhi();
        void createRenderPass();
        void createGraphicsPipeline();
        void createFrambuffer();
        void recreateSwapchainResources();

        void cleanupRhi();
    };

} // LearnVulkan


#endif//LEARNVULKAN_RENDER_SYSTEM_HPP

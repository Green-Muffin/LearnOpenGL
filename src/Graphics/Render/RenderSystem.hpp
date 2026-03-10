//
// Created by GreenMuffin on 2025/3/4.
//

#ifndef LEARNVULKAN_RENDER_SYSTEM_HPP
#define LEARNVULKAN_RENDER_SYSTEM_HPP

#include "Graphics/interface/rhi.h"
#include "Passes/UiPass.hpp"
#include "RenderResource.hpp"
#include "RenderGraph.hpp"
#include "RenderStage.hpp"
#include "Shader.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <memory>

namespace LearnVulkan {

    struct RenderSystemInitInfo {
        GLFWwindow* window;
        uint32_t width;
        uint32_t height;
    };

    class RenderSystem {
        friend class Application;
    public:
        ShaderLibrary shader_library;
        std::shared_ptr<RenderResource> render_resource{ nullptr };

        RenderSystem() = default;
        ~RenderSystem() = default;

        void initialize(RenderSystemInitInfo init_info);
        void shutdown();        
        void draw();

        void render();

    private:
        RenderSystemInitInfo init_info_{};
        std::shared_ptr<gfx::RHI> rhi_{ nullptr };


        std::shared_ptr<RenderGraph> render_graph_{ nullptr };


        UiPass ui_pass_;
    };

} // LearnVulkan


#endif//LEARNVULKAN_RENDER_SYSTEM_HPP

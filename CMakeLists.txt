cmake_minimum_required(VERSION 3.10)

project(LearnVulkan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK not found!")
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glm)

add_library(glad STATIC 3rdparty/glad/src/glad.c
        3rdparty/stb/stb_image.cpp
)

add_library(imgui STATIC
        3rdparty/imgui/imgui.cpp
        3rdparty/imgui/imgui_demo.cpp
        3rdparty/imgui/imgui_draw.cpp
        3rdparty/imgui/imgui_tables.cpp
        3rdparty/imgui/imgui_widgets.cpp
        3rdparty/imgui/backends/imgui_impl_glfw.cpp
        3rdparty/imgui/backends/imgui_impl_opengl3.cpp
        3rdparty/imgui/backends/imgui_impl_vulkan.cpp
)

target_include_directories(imgui PUBLIC
        3rdparty/imgui
        3rdparty/imgui/backends
        ${Vulkan_INCLUDE_DIRS}
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(imgui PUBLIC glfw glad ${Vulkan_LIBRARIES})

add_executable(LearnVulkan
        3rdparty/stb/stb_image.cpp
        3rdparty/stb/stb_image.h
        src/Application.cpp
        src/Application.hpp
        src/LearnVulkan.cpp
        src/Layer.cpp
        src/Layer.hpp
        src/LayerStack.cpp
        src/LayerStack.hpp
        src/Layers/LightLayer.cpp
        src/Layers/LightLayer.hpp
        src/Layers/TestVulkanLayer.cpp
        src/Layers/TestVulkanLayer.hpp
        src/Gui/MainPanel.cpp
        src/Gui/MainPanel.hpp
        src/Gui/Panel.hpp
        src/Gui/UiSystem.cpp
        src/Gui/UiSystem.hpp
        src/Graphics/interface/render_type.h
        src/Graphics/interface/rhi.h
        src/Graphics/interface/rhi_struct.h
        src/Graphics/interface/vulkan/vulkan_rhi.cpp
        src/Graphics/interface/vulkan/vulkan_rhi.h
        src/Graphics/interface/vulkan/vulkan_rhi_resource.h
        src/Graphics/interface/vulkan/vulkan_util.cpp
        src/Graphics/interface/vulkan/vulkan_util.h
        src/Graphics/interface/vulkan/vulkan_vma.cpp
        src/Graphics/Render/RenderSystem.cpp
        src/Graphics/Render/RenderSystem.hpp
        src/Graphics/Render/Passes/UiPass.cpp
        src/Graphics/Render/Passes/UiPass.hpp
        src/Graphics/Render/Shader.cpp
        src/Graphics/Render/Shader.hpp
        src/Graphics/Render/Texture.cpp
        src/Graphics/Render/Texture.hpp
        src/Graphics/Render/Renderer.cpp
        src/Graphics/Render/Renderer.hpp
        src/Graphics/Render/RenderGraph.cpp
        src/Graphics/Render/RenderGraph.hpp
        src/Graphics/Render/RenderResource.cpp
        src/Graphics/Render/RenderResource.hpp
        src/Graphics/Render/RenderStage.cpp
        src/Graphics/Render/RenderStage.hpp
        src/Graphics/Render/RenderType.hpp
)

target_include_directories(LearnVulkan PRIVATE 
    3rdparty/glfw/include 
    3rdparty/glad/include
    3rdparty/imgui
    3rdparty/imgui/backends
    3rdparty/glm
    3rdparty/stb
    3rdparty/vulkanmemoryallocator/include
    src
    ${Vulkan_INCLUDE_DIRS}
)

target_include_directories(glad PRIVATE 
    3rdparty/glad/include 
    3rdparty/glad/src
)

target_link_libraries(LearnVulkan PRIVATE glad glfw imgui ${Vulkan_LIBRARIES})

if(APPLE)
    target_link_libraries(LearnVulkan PRIVATE "-framework OpenGL")
endif()

cmake_minimum_required(VERSION 3.10)

project(LearnOpenGL)

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
        3rdparty/stb/stb_image.cpp)

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

add_executable(LearnOpenGL
        src/main.cpp
        src/renderer.cpp
        src/renderer.h
        src/vertex_buffer.cpp
        src/vertex_buffer.h
        src/index_buffer.cpp
        src/index_buffer.h
        src/vertex_buffer_layout.h
        src/vertex_array.cpp
        src/vertex_array.h
        src/shader.cpp
        src/shader.h
        3rdparty/stb/stb_image.cpp
        3rdparty/stb/stb_image.h
        src/texture.cpp
        src/texture.h
        src/tests/test.cpp
        src/tests/test.h
        src/tests/test_menu.cpp
        src/tests/test_menu.h
        src/tests/test_clear_color.cpp
        src/tests/test_clear_color.h
        src/tests/test_texture_2d.cpp
        src/tests/test_texture_2d.h
        src/redlive/redlive.cpp
        src/redlive/redlive.h
        src/tests/test_3d.cpp
        src/tests/test_3d.h
        src/camera.cpp
        src/camera.h
        src/tests/test_light.cpp
        src/tests/test_light.h
        src/tests/test_batch_render.cpp
        src/tests/test_batch_render.h
        src/LearnVulkan/Application.cpp
        src/LearnVulkan/Application.hpp
        src/LearnVulkan/LearnVulkan.cpp
        src/LearnVulkan/Layer.cpp
        src/LearnVulkan/Layer.hpp
        src/LearnVulkan/LayerStack.cpp
        src/LearnVulkan/LayerStack.hpp
        src/LearnVulkan/Layers/LightLayer.cpp
        src/LearnVulkan/Layers/LightLayer.hpp
        src/LearnVulkan/Gui/MainPanel.cpp
        src/LearnVulkan/Gui/MainPanel.hpp
        src/LearnVulkan/Gui/Panel.hpp
        src/LearnVulkan/Gui/UiSystem.cpp
        src/LearnVulkan/Gui/UiSystem.hpp
        src/LearnVulkan/Graphics/interface/render_type.h
        src/LearnVulkan/Graphics/interface/rhi.h
        src/LearnVulkan/Graphics/interface/rhi_struct.h
        src/LearnVulkan/Graphics/interface/vulkan/vulkan_rhi.cpp
        src/LearnVulkan/Graphics/interface/vulkan/vulkan_rhi.h
        src/LearnVulkan/Graphics/interface/vulkan/vulkan_rhi_resource.h
        src/LearnVulkan/Graphics/interface/vulkan/vulkan_util.cpp
        src/LearnVulkan/Graphics/interface/vulkan/vulkan_util.h
        src/LearnVulkan/Graphics/interface/vulkan/vulkan_vma.cpp
        src/LearnVulkan/Graphics/Render/RenderSystem.cpp
        src/LearnVulkan/Graphics/Render/RenderSystem.hpp
        src/LearnVulkan/Graphics/Render/Passes/UiPass.cpp
        src/LearnVulkan/Graphics/Render/Passes/UiPass.hpp
)

target_include_directories(LearnOpenGL PRIVATE 
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

target_link_libraries(LearnOpenGL PRIVATE glad glfw imgui ${Vulkan_LIBRARIES})

if(APPLE)
    target_link_libraries(LearnOpenGL PRIVATE "-framework OpenGL")
endif()

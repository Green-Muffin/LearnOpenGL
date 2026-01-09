//
// Created by GreenMuffin on 2025/10/26.
//

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

constexpr auto kScreenWidth = 800;
constexpr auto kScreenHeight = 600;
auto kWindowTitle = "OpenGL Window";

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, kWindowTitle, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    std::cout << glGetError() << std::endl;

    int fb_width = 0;
    int fb_height = 0;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    GLCall(glViewport(0, 0, fb_width, fb_height));


    constexpr float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f, // 0
        200.0f, 100.0f, 1.0f, 0.0f,  // 1
        200.0f, 200.0f, 1.0f, 1.0f,    // 2
        100.0f, 200.0f, 0.0f, 1.0f   // 3
    };

    const unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    constexpr Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    const VertexBuffer vertex_buffer(positions, 16);
    VertexArray vertex_array;
    VertexBufferLayout layout;

    layout.push<float>(2);
    layout.push<float>(2);
    vertex_array.add_buffer(vertex_buffer, layout);

    const IndexBuffer index_buffer(indices, 6 );

    const glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 720.0f, -1.0f, 1.0f);
    constexpr glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
    constexpr glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));

    const glm::mat4 mvp = projection * view * model;

    Shader shader("../res/shader/basic.shader");
    shader.bind();
    shader.set_uniform_4f("u_color", 0.2f, 0.3f, 0.8f, 1.0f);

    Texture texture("../res/textures/ChernoLogo.png");
    texture.bind();
    shader.set_uniform_mat4f("u_MVP", mvp);
    shader.set_uniform_2f("u_resolution", kScreenWidth, kScreenHeight);
    shader.set_uniform_1i("u_texture", 0);

    vertex_array.unbind();
    vertex_buffer.unbind();
    index_buffer.unbind();
    shader.unbind();

    glm::vec3 translation(200.0f, 200.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = projection * view * model;

        shader.bind();
        shader.set_uniform_mat4f("u_MVP", mvp);


        vertex_array.bind();
        index_buffer.bind();

        renderer.draw(vertex_array, index_buffer, shader);

        {
            ImGui::Begin("ImGui");
            ImGui::SliderFloat3("Tranlation", &translation.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}
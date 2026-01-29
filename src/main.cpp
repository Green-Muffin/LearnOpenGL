//
// Created by GreenMuffin on 2025/10/26.
//

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer.h"
#include "tests/test_clear_color.h"
#include "tests/test_menu.h"
#include "tests/test_texture_2d.h"

#ifdef DO_MAIN

constexpr auto kScreenWidth = 1080;
constexpr auto kScreenHeight = 720;
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


    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    constexpr Renderer renderer;

    tests::Test* current_test = nullptr;
    auto* test_menu = new tests::TestMenu(current_test);
    current_test = test_menu;

    test_menu->register_test<tests::TestClearColor>("ClearColor");
    test_menu->register_test<tests::TestTexture2D>("RenderTexture2D");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (current_test) {
            current_test->on_update(0.0f);
            current_test->on_render();
            ImGui::Begin("Test");
            if (current_test != test_menu && ImGui::Button("<-")) {
                delete current_test;
                current_test = test_menu;
            }
            current_test->on_imgui_render();
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

#endif // DO_MAIN

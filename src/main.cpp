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
#include "camera.h"
#include "tests/test_clear_color.h"
#include "tests/test_menu.h"
#include "tests/test_texture_2d.h"
#include "tests/test_3d.h"

#define DO_MAIN
#ifdef DO_MAIN

//constexpr auto kScreenWidth = 1080;
//constexpr auto kScreenHeight = 720;
//auto kWindowTitle = "OpenGL Window";
#include "redlive/redlive.h"

GLFWwindow* g_window = nullptr;

auto first_mouse = true;
auto yaw = -90.0f;
auto pitch = 0.0f;
auto last_x = static_cast<float>(kScreenWidth) / 2.0f;
auto last_y = static_cast<float>(kScreenHeight) / 2.0f;
auto fov = 45.0f;

auto delta_time = 0.0f, last_time = 0.0f;

void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    g_window = glfwCreateWindow(kScreenWidth, kScreenHeight, kWindowTitle, nullptr, nullptr);
    glfwMakeContextCurrent(g_window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    std::cout << glGetError() << std::endl;

    int fb_width = 0;
    int fb_height = 0;
    glfwGetFramebufferSize(g_window, &fb_width, &fb_height);
    GLCall(glViewport(0, 0, fb_width, fb_height));

    glfwSetCursorPosCallback(g_window, mouse_callback);
    glfwSetScrollCallback(g_window, scroll_callback);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    constexpr Renderer renderer;
    glEnable(GL_DEPTH_TEST);

    tests::Test* current_test = nullptr;
    auto* test_menu = new tests::TestMenu(current_test);
    current_test = test_menu;

    test_menu->register_test<tests::TestClearColor>("ClearColor");
    test_menu->register_test<tests::TestTexture2D>("RenderTexture2D");
    test_menu->register_test<tests::Test3D>("Render3d");

    while (!glfwWindowShouldClose(g_window)) {

        const auto current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;

        process_input(g_window);

        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (current_test) {
            current_test->on_update(delta_time);
            current_test->on_render();
            ImGui::Begin("Test");
            if (current_test != test_menu && ImGui::Button("<-")) {
                current_test->on_end();
                delete current_test;
                current_test = test_menu;
            }
            current_test->on_imgui_render();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Camera::move_depth(kCameraMoveSpeed * delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Camera::move_depth(-kCameraMoveSpeed * delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Camera::move_horizontal(kCameraMoveSpeed * delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Camera::move_horizontal(-kCameraMoveSpeed * delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
    const auto x = static_cast<float>(x_pos);
    const auto y = static_cast<float>(y_pos);

    if (first_mouse) {
        last_x = x;
        last_y = y;
        first_mouse = false;
    }

    float x_offset = x - last_x;
    float y_offset = y - last_y;
    last_x = x;
    last_y = y;

    float sensitivity = 0.02f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    Camera::set_front(front);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    fov -= static_cast<float>(y_offset);
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > 45.0f) {
        fov = 45.0f;
    }
    Camera::set_fov(fov);
}

#endif // DO_MAIN

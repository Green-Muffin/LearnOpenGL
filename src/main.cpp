//
// Created by GreenMuffin on 2025/10/26.
//

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

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
    glViewport(0, 0, fb_width, fb_height);

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    constexpr float positions[8] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };

    const unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexBuffer vertex_buffer(positions, 8);
    VertexArray vertex_array;
    VertexBufferLayout layout;
    layout.push<float>(2);

    vertex_array.add_buffer(vertex_buffer, layout);

    IndexBuffer index_buffer(indices, 6 );

    Shader shader("../res/basic.shader");
    shader.bind();
    shader.set_uniform_4f("u_color", 0.2f, 0.3f, 0.8f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}
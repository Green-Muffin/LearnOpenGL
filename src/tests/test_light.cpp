//
// Created by GreenMuffin(Redlive) on 2026/01/30.
//

#include "test_light.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include "camera.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace tests {

	TestLight::TestLight() {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        obj_vao_ = create_scope<VertexArray>();
        light_vao_ = create_scope<VertexArray>();
        vertex_buffer_ = create_scope<VertexBuffer>(vertices, sizeof(vertices));

        VertexBufferLayout obj_layout, light_layout;
        obj_layout.push<float>(3);
        obj_layout.push<float>(2);
        light_layout.push<float>(3, 5);

		obj_vao_->add_buffer(*vertex_buffer_, obj_layout);
		light_vao_->add_buffer(*vertex_buffer_, light_layout);

        obj_shader_ = create_scope<Shader>("obj.shader");
        light_shader_ = create_scope<Shader>("light.shader");

        glm::mat4 model = glm::mat4(1.0f);
        obj_model_ = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view_ = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection_ = glm::perspective(glm::radians(45.0f), static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight), 0.1f, 100.0f);

        obj_mvp_ = projection_ * view_ * obj_model_;

        obj_shader_->set_uniform_mat4f("u_mvp", obj_mvp_);
        obj_shader_->set_uniform_3f("obj_color", 1.0f, 0.5f, 0.31f);
        obj_shader_->set_uniform_3f("light_color", 1.0f, 1.0f, 1.0f);


		glm::vec3 lightPos(1.2f, 1.0f, -2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        light_model_ = glm::scale(model, glm::vec3(0.2f));

		light_mvp_ = projection_ * view_ * light_model_;
        light_shader_->set_uniform_mat4f("u_mvp", light_mvp_);
	}

	TestLight::~TestLight() {

	}

	void TestLight::on_start() {
		glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

    void TestLight::on_render() {
        view_ = Camera::View();
        obj_mvp_ = projection_ * view_ * obj_model_;
        light_mvp_ = projection_ * view_ * light_model_;

        obj_shader_->bind();
        obj_shader_->set_uniform_mat4f("u_mvp", obj_mvp_);
        renderer.draw(*obj_vao_, *obj_shader_);

        light_shader_->bind();
        light_shader_->set_uniform_mat4f("u_mvp", light_mvp_);
		glDisable(GL_DEPTH_TEST);
        renderer.draw(*light_vao_, *light_shader_);
		glEnable(GL_DEPTH_TEST);
    }

} // tests
//
// Created by GreenMuffin on 2026/1/10.
//

#include "test_clear_color.h"

#include "imgui.h"
#include "renderer.h"
#include "glad/glad.h"

namespace tests {

    TestClearColor::TestClearColor() : clear_color_{0.2f, 0.3f, 0.8f, 1.0f} {

    }

    TestClearColor::~TestClearColor() {

    }

    void TestClearColor::on_update(float delta_time) {

    }

    void TestClearColor::on_render() {
        GLCall(glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::on_imgui_render() {
        ImGui::ColorEdit4("Clear Color", clear_color_);
    }

}

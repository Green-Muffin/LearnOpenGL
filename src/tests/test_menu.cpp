//
// Created by GreenMuffin on 2026/1/10.
//

#include "test_menu.h"


#include "imgui.h"

namespace tests {
    TestMenu::TestMenu(Test*& current_test)
        : current_test_(current_test) {
    }

    TestMenu::~TestMenu() = default;

    void TestMenu::on_imgui_render() {
        ImGui::Text("Test Menu");

        for (auto& test : tests_) {
            if (ImGui::Button(test.first.c_str())) {
                current_test_ = test.second();
            }
        }
    }

}

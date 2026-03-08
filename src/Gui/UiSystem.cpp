//
// Created by GreenMuffin on 2026/2/27.
//

#include "UiSystem.hpp"

#include "imgui.h"

namespace LearnVulkan {

	void UiSystem::initialize() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	}

    void UiSystem::render() {
        _mainPanel.onGuiRender();
    }

    void UiSystem::shutdown() {
        ImGui::DestroyContext();
    }

} // LearnVulkan
//
// Created by GreenMuffin on 2026/2/27.
//

#ifndef LEARNVULKAN_UI_SYSTEM_HPP
#define LEARNVULKAN_UI_SYSTEM_HPP

#include "Gui/MainPanel.hpp"

#include <GLFW/glfw3.h>


namespace LearnVulkan {
	class UiSystem{
	public:
		UiSystem() = default;
		~UiSystem() = default;

		void initialize();
		void render();
		void shutdown();
	private:
		MainPanel _mainPanel;
	};
} // LearnVulkan

#endif//LEARNVULKAN_UI_SYSTEM_HPP

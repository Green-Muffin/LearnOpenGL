//

// Created by GreenMuffin on 2026/2/27.
//

#ifndef LEARNOPENGL_LEARNOPENGL_HPP
#define LEARNOPENGL_LEARNOPENGL_HPP

#include "Graphics/Render/RenderSystem.hpp"
#include "Gui/UiSystem.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>
#include <memory>
#include <string>

namespace LearnVulkan {

	class Application {
	public:
		Application();
		~Application();

		void run();

	private:
		GLFWwindow* _nativeWindow{ nullptr };
		std::unique_ptr<RenderSystem> _renderSystem;
		std::unique_ptr<UiSystem> _uiSystem;

		uint32_t _windowWidth{ 1200 };
		uint32_t _windowHeight{ 900 };

		bool _isCustomTitleBar{false};

		void initWindow();
		void initRender();
		void initImGui();

		void cleanupWindow();
		void cleanupRender();
		void cleanupImGui();
	};


	Application* createApplication();

} // LearnVulkan

#endif // !LEARNOPENGL_LEARNOPENGL_HPP

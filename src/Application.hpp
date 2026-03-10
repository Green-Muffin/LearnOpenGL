//

// Created by GreenMuffin on 2026/2/27.
//

#ifndef LEARNOPENGL_LEARNOPENGL_HPP
#define LEARNOPENGL_LEARNOPENGL_HPP

#include "Graphics/Render/RenderSystem.hpp"
#include "Gui/UiSystem.hpp"
#include "LayerStack.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>
#include <memory>
#include <string>

namespace LearnVulkan {

	class Application {
	public:
		std::unique_ptr<RenderSystem> renderSystem;

		Application();
		~Application();

		void run();

		static Application& self();

	private:
		static Application* instance_;

		GLFWwindow* _nativeWindow{ nullptr };
		std::unique_ptr<UiSystem> _uiSystem;

		LayerStack _layerStack;

		float _lastTime{ 0.0f };
		uint32_t _windowWidth{ 1200 };
		uint32_t _windowHeight{ 900 };

		bool _isCustomTitleBar{false};

		void initWindow();
		void initRender();
		void initImGui();
		void initLayers();

		void cleanupWindow();
		void cleanupRender();
		void cleanupImGui();
		void cleanupLayers();
	};


	Application* createApplication();

} // LearnVulkan

#endif // !LEARNOPENGL_LEARNOPENGL_HPP

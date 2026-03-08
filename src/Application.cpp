//
// Created by GreenMuffin on 2026/2/27.
//

#include "Application.hpp"

#include "Graphics/interface/vulkan/vulkan_rhi.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace LearnVulkan {

	Application* createApplication() {
		return new Application();
	}

	Application::Application() {
		initWindow();
		initImGui();
		initRender();
	}

	Application::~Application() {
		cleanupRender();
		cleanupImGui();
		cleanupWindow();
	}

	void Application::run() {
		while (!glfwWindowShouldClose(_nativeWindow)) {
			_renderSystem->drawFrame();
			glfwPollEvents();
		}

		if (_renderSystem->_rhi && _renderSystem->_rhi->getGraphicsQueue()) {
			_renderSystem->_rhi->queueWaitIdle(_renderSystem->_rhi->getGraphicsQueue());
		}
	}

	void Application::initWindow() {
		if (!glfwInit()) {
			throw std::runtime_error("failed to init glfw");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		if (_isCustomTitleBar)	{
			glfwWindowHint(GLFW_DECORATED, GL_FALSE);
		}

		_nativeWindow = glfwCreateWindow(static_cast<int>(_windowWidth), static_cast<int>(_windowHeight), "LearnVulkan", nullptr, nullptr);
		if (!_nativeWindow) {
			throw std::runtime_error("failed to create window");
		}
	}
	
	void Application::initRender() {
		_renderSystem = std::make_unique<RenderSystem>();
		_renderSystem->initialize(_nativeWindow);
	}

	void Application::initImGui() {
		_uiSystem = std::make_unique<UiSystem>();
	    _uiSystem->initialize();
	}

	void Application::cleanupWindow() {
		if (_nativeWindow) {
			glfwDestroyWindow(_nativeWindow);
			_nativeWindow = nullptr;
		}
		glfwTerminate();
	}

	void Application::cleanupRender() {
		_renderSystem->shutdown();
	}

	void Application::cleanupImGui() {
		_uiSystem->shutdown();
	}

} // LearnVulkan

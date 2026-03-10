//
// Created by GreenMuffin on 2026/2/27.
//

#include "Application.hpp"

#include "Graphics/interface/vulkan/vulkan_rhi.h"

#include "Layers/TestVulkanLayer.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace LearnVulkan {

	Application* Application::instance_ = nullptr;

	Application* createApplication() {
		return new Application();
	}

	Application::Application() {
		if (instance_) {
			std::runtime_error("Application instance already exists!");
		}

		instance_ = this;

		initWindow();
		initImGui();
		initRender();
		initLayers();
	}

	Application::~Application() {
		cleanupRender();
		cleanupImGui();
		cleanupWindow();
		cleanupLayers();
	}

	Application& Application::self() {
		return *instance_;
	}

	void Application::run() {
		while (!glfwWindowShouldClose(_nativeWindow)) {
			glfwPollEvents();

			const float currentTime = static_cast<float>(glfwGetTime());
			const float deltaTime = currentTime - _lastTime;
			_layerStack.onLogicTick(deltaTime);
			_layerStack.onrenderTick();
			_lastTime = currentTime;
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
		renderSystem = std::make_unique<RenderSystem>();
		RenderSystemInitInfo initInfo{ _nativeWindow, _windowWidth, _windowHeight };
		renderSystem->initialize(initInfo);
	}

	void Application::initImGui() {
		_uiSystem = std::make_unique<UiSystem>();
	    _uiSystem->initialize();
	}

	void Application::initLayers() {
		_layerStack.pushLayer(new TestVulkanLayer("TestVulkanLayer"));
	}

	void Application::cleanupWindow() {
		if (_nativeWindow) {
			glfwDestroyWindow(_nativeWindow);
			_nativeWindow = nullptr;
		}
		glfwTerminate();
	}

	void Application::cleanupRender() {
		renderSystem->shutdown();
	}

	void Application::cleanupImGui() {
		_uiSystem->shutdown();
	}

	void Application::cleanupLayers() {
		_layerStack.clearLayers();
	}

} // LearnVulkan

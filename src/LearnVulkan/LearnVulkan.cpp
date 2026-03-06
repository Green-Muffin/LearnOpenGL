//
// Created by GreenMuffin on 2026/2/27.
//

#include "Application.hpp"

#define doLearnVulkan
#ifdef doLearnVulkan

int main() {
	LearnVulkan::Application* app = LearnVulkan::createApplication();
	app->run();
	delete app;
}

#endif // doLearnVulkan

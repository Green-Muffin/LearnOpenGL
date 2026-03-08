//
// Created by GreenMuffin on 2026/2/27.
//

#ifndef LEARNOPENGL_LAERY_STACK_HPP
#define LEARNOPENGL_LAERY_STACK_HPP

#include "Layer.hpp"

#include <vector>

namespace LearnVulkan {

	class LayerStack {
	public:
		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void popLayer(const std::string& name);
		void clearLayers();
		Layer* getLayer(const std::string& name);
		
		void onLogicTick(float deltaTime);
		void onrenderTick();

	private:
		std::vector<Layer*> _layers;
	};

} // LearnVulkan

#endif//LEARNOPENGL_LAERY_STACK_HPP

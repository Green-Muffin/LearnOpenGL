//
// Created by GreenMuffin on 2026/2/27.
//

#ifndef LEARNOPENGL_LIGHT_LAYER_HPP
#define LEARNOPENGL_LIGHT_LAYER_HPP

#include "Layer.hpp"

namespace LearnVulkan {

	class LightLayer : public Layer {
	public:
		LightLayer(const std::string& name) : Layer(name) { }

		void onRenderTick() override;
	};

} // LearnVulkan

#endif//LEARNOPENGL_LIGHT_LAYER_HPP



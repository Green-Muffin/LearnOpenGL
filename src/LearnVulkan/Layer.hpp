//
// Created by GreenMuffin on 2026/2/27.
//

#ifndef LEARNOPENGL_LAYER_HPP
#define LEARNOPENGL_LAYER_HPP

#include <string>

namespace LearnVulkan {

	class Layer {
	public:
		explicit Layer(const std::string& name) : _name(name) { }
		virtual ~Layer() { }

		virtual void onAttach() { }
		virtual void onDetach() { }
		virtual void onLogicTick(float deltaTime) { }
		virtual void onRenderTick() { }

		const std::string& getName() { return _name; }

	private:
		std::string _name;
	};

} // LearnVulkan

#endif//LEARNOPENGL_LAYER_HPP

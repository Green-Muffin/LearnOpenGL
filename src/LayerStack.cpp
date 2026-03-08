//
// Created by GreenMuffin on 2026/2/27.
//

#include "LayerStack.hpp"

namespace LearnVulkan {

	void LayerStack::pushLayer(Layer* layer) {
		_layers.push_back(layer);
		layer->onAttach();
	}

	void LayerStack::popLayer(Layer* layer) {
		auto it = std::find(_layers.begin(), _layers.end(), layer);
		if (it != _layers.end()) {
			(*it)->onDetach();
			_layers.erase(it);
		}
	}

	void LayerStack::popLayer(const std::string& name) {
		auto layer = getLayer(name);
		if (layer == nullptr) return;
		popLayer(layer);
	}

	Layer* LayerStack::getLayer(const std::string& name) {
		auto it = std::find_if(_layers.begin(), _layers.end(),
			[&name](Layer* layer) {
				return layer && layer->getName() == name;
			});
		return (it != _layers.end()) ? *it : nullptr;
	}

	void LayerStack::clearLayers() {
		for (auto& layer : _layers) {
			layer->onDetach();
		}
		std::vector<Layer*>().swap(_layers);
	}

	void LayerStack::onLogicTick(const float deltaTime) {
		for (auto& layer : _layers) {
			layer->onLogicTick(deltaTime);
		}
	}

	void LayerStack::onrenderTick() {
		for (auto& layer : _layers) {
			layer->onRenderTick();
		}
	}

} // LearnVulkan
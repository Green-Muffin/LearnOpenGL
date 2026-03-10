//
// Created by GreenMuffin on 2026/3/8.
//

#ifndef LEARNVULKAN_RENDER_HPP
#define LEARNVULKAN_RENDER_HPP

#include "RenderType.hpp"

#include "glm/glm.hpp"

#include <vector>

namespace LearnVulkan {

	class Renderer {
	public:
		static void draw_sprite();
		static void draw_sprite(const DrawSpriteContext& context);
		static void draw_model();
	};
}

#endif // !LEARNVULKAN_RENDER_HPP

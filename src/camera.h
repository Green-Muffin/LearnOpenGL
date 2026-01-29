//
// Created by GreenMuffin(Redlive) on 2026/01/29.
//

#ifndef LEARNOPENGL_CMAERA_H
#define LEARNOPENGL_CMAERA_H

#include "glm/glm.hpp"

class Camera {
public:
	static const glm::mat4& View();
	static void move_horizontal(float value);
	static void move_vertical(float value);
	static void move_depth(float value);

	static void set_front(const glm::vec3& front);
	static void set_fov(float fov);
	static float get_fov();

private:
	static glm::vec3 camera_pos_, camera_front_, camera_up_;
	static float fov_;
};

#endif // LEARNOPENGL_CMAERA_H

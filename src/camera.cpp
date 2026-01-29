//
// Created by GreenMuffin(Redlive) on 2026/01/29.
//

#include "camera.h"
#include <glm/ext/matrix_transform.hpp>

#include <iostream>

glm::vec3 Camera::camera_pos_ = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 Camera::camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::camera_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
float Camera::fov_ = 45.0f;

const glm::mat4& Camera::View() {
	return glm::lookAt(camera_pos_, camera_front_ + camera_pos_, camera_up_);
}

void Camera::move_horizontal(float value) {
	camera_pos_ += glm::normalize(glm::cross(camera_front_, camera_up_)) * value;
}

void Camera::move_vertical(float value) {
	camera_pos_ += camera_up_ * value;
}

void Camera::move_depth(float value) {
	camera_pos_ += camera_front_ * value;
}

void Camera::set_front(const glm::vec3& front) {
	camera_front_ = front;
}

void Camera::set_fov(const float fov) {
	fov_ = fov;
}

float Camera::get_fov() {
	return fov_;
}
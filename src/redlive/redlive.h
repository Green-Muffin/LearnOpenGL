//
// Created by GreenMuffin(Redlive) by 2026/01/29.
//

#pragma once

#include "renderer.h"
#include "glfw/glfw3.h"
#include <memory>

using uint = unsigned int;

const uint kScreenWidth = 1280;
const uint kScreenHeight = 720;
const auto kWindowTitle = "OpenGL Window";

const float kCameraMoveSpeed = 2.5f;

extern GLFWwindow* g_window;

template <typename T>
using scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr scope<T> create_scope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
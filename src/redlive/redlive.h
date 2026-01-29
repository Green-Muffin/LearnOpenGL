//
// Created by GreenMuffin(Redlive) by 2026/01/29.
//

#pragma once

#include "glfw/glfw3.h"

using uint = unsigned int;

const uint kScreenWidth = 1280;
const uint kScreenHeight = 720;
const auto kWindowTitle = "OpenGL Window";

const float kCameraMoveSpeed = 2.5f;

extern GLFWwindow* g_window;
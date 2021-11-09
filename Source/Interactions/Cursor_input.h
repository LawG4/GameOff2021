/*!********************************************************************************************************
\File          : Cursor_input.cpp
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#pragma once
#include "Vulkan.h"
// GLFW inclusion before vk headers causes error with vk
#include <GLFW/glfw3.h>

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

/*!********************************************************************************************************
\File          : Keyboard_input.h
\Copyright     : GPL-3.0 License
\Brief         : Adds a header for window keyboard input
\Contributors  : Freddie M,
 *********************************************************************************************************/

#pragma once

#include "Vulkan.h"
// GLFW inclusion before vk headers causes error with vk
#include <GLFW/glfw3.h>
#include "Player_object.h"

// Actual code here
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

/*!********************************************************************************************************
\File          : Cursor_input.h
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#pragma once
#include "Vulkan.h"
// GLFW inclusion before vk headers causes error with vk
#include <GLFW/glfw3.h>

// Create cursor  class
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// Function to move central position to top left
int vertex_to_zero(int vertex, int max_vertex);

// Global windowdimen (window dimension) array to hold window dimension data
extern uint32_t windowdimen[2];

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

// Function to move central position to top left
int vertex_to_zero(int vertex, int max_vertex);

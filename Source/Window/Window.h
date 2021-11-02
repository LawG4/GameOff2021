/*!********************************************************************************************************
\File          : Window.h
\Copyright     : MIT License
\Brief         : Adds a header for window abstraction through GLFW
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

// Before including GLFW we need to include the Vulkan headers, this is so that GLFW will know to declare
// Vulkan types
#include "Vulkan.h"

#include <GLFW/glfw3.h>

// Opaque handle to the GLFW window
extern GLFWwindow *window;

///< summary> Creates the GLFW window </summary>
///< returns>True if the window was created successfully </returns>
bool initWindow();

/*!********************************************************************************************************
\File          : Window.h
\Copyright     : GPL-3.0 License
\Brief         : Adds a header for window abstraction through GLFW
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once

// Before including GLFW we need to include the Vulkan headers, this is so that GLFW will know to declare
// Vulkan types
#include "Vulkan.h"

#include <GLFW/glfw3.h>

extern GLFWwindow *window;

/// <summary> Creates a GLFW window </summary>
/// <param name="windowTitle"> Window title for the window being created </param>
/// <param name="isFullScreen"> Is the window going to be full screen </param>
/// <param name="isFullScreen"> The width of the window if not full screen</param>
/// <param name="isFullScreen"> The height of the window if not full screen </param>
/// <returns>True if the window was created successfully </returns>
bool initWindow(const char *windowTitle, uint32_t width, uint32_t height, bool isFullScreen);

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

// We also need to tell glfw to use glad, that way extensions know to be laoded
#include <glad/glad.h>

#include <GLFW/glfw3.h>

// Pointer to the current window
extern GLFWwindow **window;
// Opaque handle to the GLFW window
extern GLFWwindow *gameWindow;
extern GLFWwindow *launcher;

/// <summary> Creates a GLFW window </summary>
/// <param name="windowTitle"> Window title for the window being created </param>
/// <param name="isFullScreen"> Is the window going to be full screen </param>
/// <param name="isFullScreen"> The width of the window if not full screen</param>
/// <param name="isFullScreen"> The height of the window if not full screen </param>
/// <param name="isLauncher"> Are we creating a launcher </param>
/// <param name="useVulkan"> Was adaquate Vulkan support detected </param>
/// <returns>True if the window was created successfully </returns>
bool initWindow(const char *windowTitle, uint32_t width, uint32_t height, bool isFullScreen = false,
                bool isLauncher = true, bool useVulkan = false);

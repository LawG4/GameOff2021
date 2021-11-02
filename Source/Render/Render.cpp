/*!********************************************************************************************************
\File          : Render.cpp
\Copyright     : MIT License
\Brief         : Manages the rendering of the current frame handles API abstraction
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Render.h"

std::unordered_map<GLFWwindow*, bool> windowUsingVulkan;

void setClearColor(GLFWwindow* context, float r, float g, float b)
{
    // Look up if the current window is using Vulkan or GL
    if (windowUsingVulkan[context]) {
        // not yet implemented
        Log.error("Vulkan set clear colour not implemented yet");
    } else {
        // Set the clear colour
        glClearColor(r, g, b, 1.0);
    }
}

void clearFrame(GLFWwindow* context)
{
    if (windowUsingVulkan[context]) {
        // Not yet implemented
        Log.error("Vulkan clear frame not yet implemented");
    } else {
        // Clear the colour attachment
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

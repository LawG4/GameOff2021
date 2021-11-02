/*!********************************************************************************************************
\File          : LauncherCallback.cpp
\Copyright     : MIT License
\Brief         : Adds the key callbacks for the GLFW window managing the launcher
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Window.h"

void launcherKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // If the user presses esc then lets escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // Close the launcher window
        glfwSetWindowShouldClose(launcher, GLFW_TRUE);
    }

    // If the user presses enter then let's make the launcher
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        // Evaluate suitability for using Vulkan or GL for the actual game
        bool useVulkan = isVulkanSuitable();

        Log.info("Creating new game window!");
        // Create a new window
    }
}

/*!********************************************************************************************************
\File          : Window.cpp
\Copyright     : MIT License
\Brief         : Adds a definitions for the declared functions and variables in Window.h
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Window.h"
#include "Log.h"
#include "Render.h"

GLFWwindow *window;

// Tracks if GLFW has been initialised.
bool glfwActive = false;

// Tracks the focused GLFW window
bool isLauncherFocused = false;

void glfwErrorCallback(int error, const char *desc) { Log.error("GLFW ERROR : {}", desc); }

bool initWindow(const char *windowTitle, uint32_t width, uint32_t height, bool isFullScreen)
{
    // Has GLFW been initialised yet
    if (!glfwActive) {
        Log.info("Attempting to start GLFW");
        if (!glfwInit()) {
            Log.error("Could not start GLFW");
        }

        // Set up a GLFW error callback
        glfwSetErrorCallback(glfwErrorCallback);
    }

    // Tell GLFW to use no specific API as Vulkan will handle this
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create the window
    window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    glfwShowWindow(window);
    return true;
}

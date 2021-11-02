/*!********************************************************************************************************
\File          : Window.cpp
\Copyright     : MIT License
\Brief         : Adds a definitions for the declared functions and variables in Window.h
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Window.h"
#include "Log.h"

GLFWwindow **window;
GLFWwindow *gameWindow;
GLFWwindow *launcher;

// Tracks if GLFW has been initialised.
bool glfwActive = false;

// Tracks the focused GLFW window
bool isLauncherFocused = false;

void glfwErrorCallback(int error, const char *desc) { Log.error("GLFW ERROR : {}", desc); }

bool initWindow(const char *windowTitle, uint32_t width, uint32_t height, bool isFullScreen, bool isLauncher,
                bool useVulkan)
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

    // Are we using the launcher?
    isLauncherFocused = isLauncher;
    window = &launcher;
    if (!isLauncher) window = &gameWindow;

    // If we are creating the window for the launcher or for not Vulkan then we are using GL
    bool useGL = !useVulkan;
    if (isLauncher) useGL = true;

    // Set the window hints depending on the API
    if (useGL) {
        Log.info("Using GL for {} window", isLauncher ? "launcher" : "game");
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    } else {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    // Create the window
    *window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    glfwShowWindow(*window);

    return true;
}

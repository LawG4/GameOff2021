/*!********************************************************************************************************
\File          : Window.cpp
\Copyright     : MIT License
\Brief         : Adds a definitions for the declared functions and variables in Window.h
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Window.h"

// If the user is compiling for release mode we need to add the windows header, that way we can close the
// console for release builds
#if defined(WIN32) && defined(NDEBUG)
#include <Windows.h>
#define FORCE_CLOSE_WIN32_CONSOLE 1
#endif  // defined(WIN32) && defined(NDEBUG)

GLFWwindow *window;

bool initWindow()
{
    // If the user is in windows release mode then use windows api to close console
#ifdef FORCE_CLOSE_WIN32_CONSOLE
    FreeConsole();
#endif  // FORCE_CLOSE_WIN32_CONSOLE

    // Initialise GLFW
    if (!glfwInit()) {
        return false;
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(640, 480, "BUG", NULL, NULL);
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    return true;
}

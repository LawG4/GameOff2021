/*!********************************************************************************************************
\File          : Window.cpp
\Copyright     : MIT License
\Brief         : Adds a definitions for the declared functions and variables in Window.h
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Window.h"

GLFWwindow *window;

bool initWindow()
{
    // Initialise GLFW
    if (!glfwInit()) {
        return false;
    }

    // Create the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(640, 480, "BUG", NULL, NULL);
    glfwShowWindow(window);

    return true;
}

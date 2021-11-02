/*!********************************************************************************************************
\File          : Main.cpp
\Copyright     : MIT License
\Brief         : Adds an entry point into the game
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Log.h"
#include "Window.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (!initLog()) {
        std::cout << "Somehow the logger failed to init!\nWhat on earth happened??" << std::endl;
        return -1;
    }

    if (!initWindow()) {
        Log.error("Could not initialise GLFW window");
        return -1;
    }

    if (!initVulkan()) {
        Log.error("Could not initialise Vulkan");
        return -1;
    }

    // Enter into the windowing loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

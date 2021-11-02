/*!********************************************************************************************************
\File          : Main.cpp
\Copyright     : MIT License
\Brief         : Adds an entry point into the game
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#include "Log.h"
#include "Render.h"
#include "Window.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (!initLog()) {
        std::cout << "Somehow the logger failed to init!\nWhat on earth happened??" << std::endl;
        return -1;
    }

    // Create the launcher
    if (!initWindow("B.U.G Launcher", 720, 400, false)) {
        Log.error("Could not initialise GLFW window for the laucher");
        glfwTerminate();
        return -1;
    }

    // Ensure that the machine is capable of loading Vulkan at all
    if (!glfwVulkanSupported()) {
        Log.error("Vulkan is not supported on your device");
        return -1;
    } else {
        Log.info("GLFW Confirms a Vulkan device");
    }

    // Enter into the windowing loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // glfwSwapBuffers(window);
    }
}

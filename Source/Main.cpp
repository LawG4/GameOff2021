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
    if (!initWindow("B.U.G Launcher", 720, 400)) {
        Log.error("Could not initialise GLFW window for the laucher");
        glfwTerminate();
        return -1;
    }

    // Enter into the windowing loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwSwapBuffers(window);
    }
}

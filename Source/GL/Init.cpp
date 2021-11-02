/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Inits openGL
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "GL.h"
#include "Log.h"
#include "Window.h"

bool initGL()
{
    Log.info("Initialising OpenGL");

    glfwMakeContextCurrent(*window);
    if (!gladLoadGL()) {
        Log.error("Could not retireve GL pointers");
        return false;
    }

    // Set the swap interval
    glfwSwapInterval(1);

    // Get the framebuffer dimensions
    int width, height;
    glfwGetFramebufferSize(*window, &width, &height);
    glViewport(0, 0, width, height);

    // Set the clear colour
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    return true;
}

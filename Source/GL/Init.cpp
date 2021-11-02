/*!********************************************************************************************************
\File          : Vulkan.h
\Copyright     : MIT License
\Brief         : Inits openGL
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "GL.h"
#include "Log.h"
#include "Window.h"

bool glStarted = false;

bool initGL()
{
    if (!glStarted) {
        Log.info("Initialising OpenGL");

        if (!gladLoadGL()) {
            Log.error("Could not retireve GL pointers");
            return false;
        } else {
            Log.info("OpenGL initialised");
        }
    } else {
        Log.warn("GL already initialised");
        return true;
    }

    return true;
}

bool glSurfaceSetup(GLFWwindow* context)
{
    glfwMakeContextCurrent(context);

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

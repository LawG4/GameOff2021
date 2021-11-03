/*!********************************************************************************************************
\File          : Player.cpp
\Copyright     : MIT License
\Brief         : Keyboard input
\Contributors  : Freddie M,
 *********************************************************************************************************/

#include "Log.h"
#include "Window.h"

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        Log.info("button E pressed");
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        Log.info("button W pressed");
    }
}

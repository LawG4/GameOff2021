/*!********************************************************************************************************
\File          : Player.cpp
\Copyright     : MIT License
\Brief         : Keyboard input
\Contributors  : Freddie M,
 *********************************************************************************************************/

#include "GL.h"
#include "Log.h"
#include "Window.h"

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

        if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        Log.error("button pressed weee");
        }
         
    }
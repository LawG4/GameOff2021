/*!********************************************************************************************************
\File          : Keyboard_input.cpp
\Copyright     : MIT License
\Brief         : Keyboard input
\Contributors  : Freddie M,
 *********************************************************************************************************/

#include "Keyboard_input.h"
#include <iostream>
#include "Player_object.h"
#include "Window.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {  // E
        player_class->player_step_size++;
        Log.info(player_class->out_player_damage());
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {  // W
        Log.info("button W pressed");
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {  // A
        Log.info("button A pressed");
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {  // S
        Log.info("button S pressed");
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {  // D
        Log.info("button D pressed");
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {  // Q
        Log.info("button Q pressed");
    }
}

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
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        player_class->player_step_size++;
        Log.info(player_class->out_player_damage());
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        Log.info("button W pressed");
    }
}

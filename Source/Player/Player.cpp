/*!********************************************************************************************************
\File          : Player.cpp
\Copyright     : MIT License
\Brief         : Keyboard input
\Contributors  : Freddie M,
 *********************************************************************************************************/

#include <iostream>
#include "Player_object.h"
#include "Window.h"

void key_callback(GLFWwindow* window, player* player_class, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        player_class->player_step_size++;
        Log.info(player_class->player_step_size);
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        Log.info("button W pressed");
    }
}

/*
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

        std::cout << "help";
}

*/

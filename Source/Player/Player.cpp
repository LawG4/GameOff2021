/*!********************************************************************************************************
\File          : Player.cpp
\Copyright     : MIT License
\Brief         : Keyboard input
\Contributors  : Freddie M,
 *********************************************************************************************************/

#include <iostream>
//#include "Log.h"
#include "Window.h"

/*
=== Plan ===

Make player class/struct object thing
- 3 parts (Physical[movements],Actions[attacks, maybe merge with physical??],General updates[e.g health
updates, all all player data e.g attack pwoer ect])

- Detect when playable, so when paused or un paused
- Take key commands and

*/

class player
{  // Player class to hold player data and basic player functions
   public:
    int player_health;
    int player_damage;
    int player_step_size;

    player()
    {  // Player constructor to create base player data values
        player_health = 100;
        player_damage = 5;
        player_step_size = 1;
    }
};

player build_player()
{
    player* player_obejct = new player;
    return player_obejct * ();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)  //, player* player_class)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        player_class.player_step_size++;
        Log.info(player_class.player_step_size);
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

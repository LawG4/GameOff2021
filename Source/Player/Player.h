/*!********************************************************************************************************
\File          : Player.h
\Copyright     : MIT License
\Brief         : Adds a header for window input
\Contributors  : Freddie M,
 *********************************************************************************************************/

#pragma once

// We also need to tell glfw to use glad, that way extensions know to be laoded

//#include <GLFW/glfw3.h>

// Actual code here
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods);  //, player* player_class);

// Build player class
void build_player()

  // static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

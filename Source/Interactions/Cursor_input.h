/*!********************************************************************************************************
\File          : Cursor_input.h
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#pragma once
#include "Vulkan.h"
// GLFW inclusion before vk headers causes error with vk
#include <GLFW/glfw3.h>
#include "EntryMenu.h"

// Cursor callback
void menu_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// Create cursor click feedback
void menu_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

extern EntryMenu* MainMenu;

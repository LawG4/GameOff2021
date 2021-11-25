/*!********************************************************************************************************
\File          : Cursor_input.h
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Log.h"
#include "collision.h"

// Callback for cursor buttons
void menu_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (MainMenu->IS_MENU_ACTIVE) {
            MainMenu->cursor_click(button);
        } else if (PauseMenu->IS_MENU_ACTIVE) {
            PauseMenu->cursor_click(button);
        }
    }
}
// Callback for cursor position
void menu_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (MainMenu->IS_MENU_ACTIVE) {
        MainMenu->cursor_update(xpos, ypos);
    } else if (PauseMenu->IS_MENU_ACTIVE) {
        PauseMenu->cursor_update(xpos, ypos);
    }
}

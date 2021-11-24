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

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MainMenu->cursor_update(xpos, ypos);
    PauseMenu->cursor_update(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (MainMenu->IS_MENU_ACTIVE) {
        Log.error("MainMenu");
        MainMenu->cursor_click(button);
    } else if (PauseMenu->IS_MENU_ACTIVE) {
        Log.error("PauseMenu");
        PauseMenu->cursor_click(button);
    }
}

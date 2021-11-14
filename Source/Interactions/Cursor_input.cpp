/*!********************************************************************************************************
\File          : Cursor_input.h
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Cursor_input.h"
#include "EntryMenu.h"

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Convert double to nice looking string
    std::string varAsString = std::to_string((int)xpos);
    varAsString = varAsString + "  " + std::to_string((int)ypos);

    // Output to log
    Log.info(varAsString);
    // Log.info(windowdimen[1]);

    if (xpos < MainMenu->x_coordinate_range[0] ||
        xpos > MainMenu->x_coordinate_range[1] && ypos < MainMenu->y_coordinate_range[0] ||
        ypos > MainMenu->y_coordinate_range[1]) {
        MainMenu->click_button();
    }
}

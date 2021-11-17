/*!********************************************************************************************************
\File          : Cursor_input.h
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Cursor_input.h"
#include "EntryMenu.h"

bool cursor_on_box = false;
bool return_box_to_normal;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Convert double to nice looking string
    std::string varAsString = std::to_string((int)xpos);
    varAsString = varAsString + "  " + std::to_string((int)ypos);

    // Log.info(varAsString);

    if (((xpos > MainMenu->x_coordinate_range[0] && xpos < MainMenu->x_coordinate_range[1])) &&
        ((ypos > MainMenu->y_coordinate_range[0] && ypos < MainMenu->y_coordinate_range[1]))) {
        if (!cursor_on_box) {
            cursor_on_box = MainMenu->shadow_button();
            return_box_to_normal = true;

            // Output to log
            // Log.error(varAsString);
        }
    } else {
        if (return_box_to_normal) {
            return_box_to_normal = MainMenu->return_to_normal();
            cursor_on_box = false;
        }
    }
}

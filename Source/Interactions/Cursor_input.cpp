/*!********************************************************************************************************
\File          : Cursor_input.h
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Cursor_input.h"

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Convert double to string
    std::string varAsString = std::to_string((int)xpos);
    varAsString = varAsString + "  " + std::to_string((int)ypos);

    Log.info(varAsString);
}

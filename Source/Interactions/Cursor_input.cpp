/*!********************************************************************************************************
\File          : Cursor_input.h
\Copyright     : GPL-3.0 License
\Brief         : Cursor input and interactions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Cursor_input.h"

// int xsize = windowdimen[0];

// Function to move central position to top left
int vertex_to_zero(int vertex, int max_vertex) { return vertex + max_vertex; }

// Declare windowdimen (window dimension) array
uint32_t windowdimen[2];

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Convert double to nice looking string
    std::string varAsString = std::to_string((int)xpos);
    varAsString = varAsString + "  " + std::to_string((int)ypos);

    // Output to log
    // Log.info(varAsString);
    // Log.info(windowdimen[1]);
}

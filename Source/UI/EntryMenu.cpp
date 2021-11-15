/*!********************************************************************************************************
\File          : EntryMenu.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creation of main menu
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Log.h"
#include "Render.h"
#include "Window.h"
#include "nlohmann/json.hpp"

#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Keyboard_input.h"
#include "Player_object.h"

#include "Objects.h"

#include <fstream>
#include <iostream>

// Function to move central position to top left
int vertex_to_zero(float vertex, float max_vertex) { return (float)((vertex + max_vertex) / 2); }

// Create a 2d square object
EntryMenu::EntryMenu()
{
    // Create pixel scale to convert vertex coord to pixel position (to use with cursor data)
    pixel_scale_X = windowdimen[0] / vertdimen[0];
    pixel_scale_Y = windowdimen[1] / vertdimen[1];

    x_coordinate_range[0] = vertex_to_zero(1.5f, 3.5f) * pixel_scale_X;
    x_coordinate_range[1] = vertex_to_zero(-1.5f, 3.5f) * pixel_scale_X;

    y_coordinate_range[0] = vertex_to_zero(1.5f, 2.0f) * pixel_scale_Y;
    y_coordinate_range[1] = vertex_to_zero(-1.5f, 2.0f) * pixel_scale_Y;

    Log.info("I am created");

    // CHANGE SO THAT RECTANGLES RENDERED BY NOT VISSIBLE
}
// Generate triangles
bool EntryMenu::click_button()

{
    RenderObject2D *Triangle = new RenderObject2D(EntryMenu::pos, EntryMenu::col);
    RenderObject2D *Triangle2 = new RenderObject2D(EntryMenu::pos2, EntryMenu::col);
    return true;
}

// Deffine windowdimen (window dimension) array
uint32_t windowdimen[2];

// Declare windowdimen (window dimension) array
float vertdimen[2];

// Build player class as GLOBAL
EntryMenu *MainMenu = new EntryMenu();

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
float vertex_to_zero(float vertex, float max_vertex) { return (vertex + max_vertex) / 2; }

// Constructor
EntryMenu::EntryMenu()
{
    // to check if shadow button has already been rand
    first_pass = true;

    // Actually deffine what each shapes vertex will be
    start_largetop = {{1.5f, -0.5f, 0.0f}, {1.5f, 0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    start_largebottom = {{1.5f, -0.5f, 0.0f}, {-1.5f, -0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    start_smalltop = {{1.46f, -0.46f, 0.0f}, {1.46f, 0.46f, 0.0f}, {-1.46f, 0.46f, 0.0f}};
    start_smallbottom = {{1.46f, -0.46f, 0.0f}, {-1.46f, -0.46f, 0.0f}, {-1.46f, 0.46f, 0.0f}};

    // Not used quit menu space
    quit_largetop = {{1.5f, -0.5f, 0.0f}, {1.5f, 0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    quit_largebottom = {{1.5f, -0.5f, 0.0f}, {-1.5f, -0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    quit_smalltop = {{1.46f, -0.46f, 0.0f}, {1.46f, 0.46f, 0.0f}, {-1.46f, 0.46f, 0.0f}};
    quit_smallbottom = {{1.46f, -0.46f, 0.0f}, {-1.46f, -0.46f, 0.0f}, {-1.46f, 0.46f, 0.0f}};

    // Colour
    colour = {{5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}};
}

// Generate triangles
void EntryMenu::load_menu(uint32_t ww, uint32_t wh)
{
    // Create pixel scale to convert vertex coord to pixel position (to use with cursor data)
    EntryMenu::pixel_scale_X = ww / vertdimen[0];
    EntryMenu::pixel_scale_Y = wh / vertdimen[1];

    // Calc what pixel coordinates you want
    EntryMenu::x_coordinate_range[0] = vertex_to_zero(-1.5f, 3.5f) * EntryMenu::pixel_scale_X;
    EntryMenu::x_coordinate_range[1] = vertex_to_zero(1.5f, 3.5f) * EntryMenu::pixel_scale_X;

    EntryMenu::y_coordinate_range[0] = vertex_to_zero(-0.5f, 2.0f) * EntryMenu::pixel_scale_Y;
    EntryMenu::y_coordinate_range[1] = vertex_to_zero(0.5f, 2.0f) * EntryMenu::pixel_scale_Y;

    // Render shapes
    Triangle = new RenderObject2D(start_largetop, colour);
    Triangle2 = new RenderObject2D(start_largebottom, colour);
}

// Update button size and colour for when hovered over
bool EntryMenu::shadow_button()
{
    // Unload big rectangle
    Triangle->isActive = false;
    Triangle2->isActive = false;
    Triangle->scheduleUBOUpdate();
    Triangle2->scheduleUBOUpdate();

    // If first time this func being rand, create new smaller traingles, afterwards just change isActive
    if (first_pass) {
        smallTriangle = new RenderObject2D(start_smalltop, colour);
        smallTriangle2 = new RenderObject2D(start_smallbottom, colour);
    } else {
        smallTriangle->isActive = true;
        smallTriangle2->isActive = true;
    }

    return true;
}

bool EntryMenu::return_to_normal()
{
    smallTriangle->isActive = false;
    smallTriangle2->isActive = false;
    Triangle->isActive = true;
    Triangle2->isActive = true;

    smallTriangle->scheduleUBOUpdate();
    smallTriangle2->scheduleUBOUpdate();
    Triangle->scheduleUBOUpdate();
    Triangle2->scheduleUBOUpdate();

    return false;
}

// Declare windowdimen (window dimension) array
float vertdimen[2];

// Initialise MainMenu object
EntryMenu *MainMenu = new EntryMenu;

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
EntryMenu::EntryMenu(void) {}

void EntryMenu ::initialise_values()
{
    // Create pixel scale to convert vertex coord to pixel position (to use with cursor data)
    EntryMenu::pixel_scale_X = windowdimen[0] / vertdimen[0];
    EntryMenu::pixel_scale_Y = windowdimen[1] / vertdimen[1];

    EntryMenu::x_coordinate_range[0] = vertex_to_zero(-1.5f, 3.5f) * EntryMenu::pixel_scale_X;
    EntryMenu::x_coordinate_range[1] = vertex_to_zero(1.5f, 3.5f) * EntryMenu::pixel_scale_X;

    EntryMenu::y_coordinate_range[0] = vertex_to_zero(-0.5f, 2.0f) * EntryMenu::pixel_scale_Y;
    EntryMenu::y_coordinate_range[1] = vertex_to_zero(0.5f, 2.0f) * EntryMenu::pixel_scale_Y;
}

// Generate triangles
bool EntryMenu::click_button()
{
    // Move deffinition out of constructor
    const std::vector<glm::vec3> pos = {{1.5f, -0.5f, 0.0f}, {1.5f, 0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    const std::vector<glm::vec3> pos2 = {{1.5f, -0.5f, 0.0f}, {-1.5f, -0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    const std::vector<glm::vec3> col = {{5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}};

    RenderObject2D *Triangle = new RenderObject2D(pos, col);
    RenderObject2D *Triangle2 = new RenderObject2D(pos2, col);
    return true;
}

// Update button size and colour for when hovered over
void EntryMenu::shadow_button() {}

// declare windowdimen (window dimension) array
uint32_t windowdimen[2];

// Declare windowdimen (window dimension) array
float vertdimen[2];

// Initialise MainMenu object
EntryMenu *MainMenu = new EntryMenu;

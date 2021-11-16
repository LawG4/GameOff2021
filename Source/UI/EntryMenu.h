/*!********************************************************************************************************
\File          : EntryMenu.h
\Copyright     : GPL-3.0 License
\Brief         : Creation of main menu
\Contributors  : Freddie M
 *********************************************************************************************************/
#pragma once

#include "Objects.h"
#include "nlohmann/json.hpp"

class EntryMenu
{
   public:
    float pixel_scale_X;
    float pixel_scale_Y;
    float x_coordinate_range[2];
    float y_coordinate_range[2];
    RenderObject2D* Triangle;
    RenderObject2D* Triangle2;

    EntryMenu();
    void initialise_values();
    bool click_button();
    void shadow_button();
};

extern EntryMenu* MainMenu;

// Global windowdimen (window dimension) array to hold window dimension data
extern uint32_t windowdimen[2];

// Global vertdimen (vertex dimension) array to hold max vertex data
extern float vertdimen[2];

// Function to move central position to top left
float vertex_to_zero(float vertex, float max_vertex);

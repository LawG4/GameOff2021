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
    RenderObject2D* smallTriangle;
    RenderObject2D* smallTriangle2;

    // --Deffine vector names--
    // Top rectangle (main shape and shadow)
    std::vector<glm::vec3> start_largetop;
    std::vector<glm::vec3> start_largebottom;
    std::vector<glm::vec3> start_smalltop;
    std::vector<glm::vec3> start_smallbottom;

    // Bottom rectangle (main shape and shadow)
    std::vector<glm::vec3> quit_largetop;
    std::vector<glm::vec3> quit_largebottom;
    std::vector<glm::vec3> quit_largetop;
    std::vector<glm::vec3> quit_largebottom;

    // --Colour--
    std::vector<glm::vec3> colour;

    EntryMenu();

    void load_menu(uint32_t ww, uint32_t wh);
    bool shadow_button();
    bool return_to_normal();
};

extern EntryMenu* MainMenu;

// Global vertdimen (vertex dimension) array to hold max vertex data
extern float vertdimen[2];

// Function to move central position to top left
float vertex_to_zero(float vertex, float max_vertex);

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
    // Move deffinition out of constructor
    const std::vector<glm::vec3> pos = {{1.5f, -0.5f, 0.0f}, {1.5f, 0.5f, 0.0f}, {-1.5f, 0.5f, 0.0f}};
    const std::vector<glm::vec3> pos2 = {{-1.5f, 0.5f, 0.0f}, {-1.5f, -0.5f, 0.0f}, {1.5f, -0.5f, 0.0f}};
    const std::vector<glm::vec3> col = {{5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}};

    float pixel_scale_X;
    float pixel_scale_Y;
    double x_coordinate_range[2];
    double y_coordinate_range[2];

    EntryMenu();
    void click_button();
};

extern EntryMenu* MainMenu;

// Create cursor  class
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// Global windowdimen (window dimension) array to hold window dimension data
extern uint32_t windowdimen[2];

// Global vertdimen (vertex dimension) array to hold max vertex data
extern float vertdimen[2];

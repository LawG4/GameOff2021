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
    bool first_pass;
    RenderObject2D* Triangle;
    RenderObject2D* Triangle2;
    RenderObject2D* smallTriangle;
    RenderObject2D* smallTriangle2;
    bool IS_MENU_ACTIVE;
    bool cursor_on_box;
    bool return_box_to_normal;

    // --Deffine vector names--
    // Top rectangle (main shape and shadow)
    std::vector<glm::vec3> start_largetop;
    std::vector<glm::vec3> start_largebottom;
    std::vector<glm::vec3> start_smalltop;
    std::vector<glm::vec3> start_smallbottom;

    // Bottom rectangle (main shape and shadow)
    std::vector<glm::vec3> quit_largetop;
    std::vector<glm::vec3> quit_largebottom;
    std::vector<glm::vec3> quit_smalltop;
    std::vector<glm::vec3> quit_smallbottom;

    // --Colour--
    std::vector<glm::vec3> colour;

    // Constructor
    EntryMenu();

    // Render primary rectangle
    void load_menu(uint32_t ww, uint32_t wh);
    // Ran each time cursor updated
    void cursor_update(double xpos, double ypos);
    // Convert primary rectangle to mini ones
    void shadow_button();
    // Return mini rectangles to large ones
    void return_to_normal();
};

extern EntryMenu* MainMenu;

// Global vertdimen (vertex dimension) array to hold max vertex data
extern float vertdimen[2];

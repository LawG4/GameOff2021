/*!********************************************************************************************************
\File          : EntryMenu.h
\Copyright     : GPL-3.0 License
\Brief         : Creation of main menu
\Contributors  : Freddie M, Lawrence
 *********************************************************************************************************/
#pragma once

#include "Sprites.h"
#include "nlohmann/json.hpp"

class EntryMenu
{
   public:
    bool first_pass;

    bool IS_MENU_ACTIVE;
    bool Load_side_button;
    bool cursor_on_box;
    bool return_box_to_normal;
    double xposition;
    double yposition;

    // Initialise memeber pointers to null
    SpriteSheet* startFrontSheet = nullptr;
    Sprite* startFront = nullptr;
    UiSpriteInstance* frontInstance = nullptr;

    SpriteSheet* startBackSheet = nullptr;
    Sprite* startBack = nullptr;
    UiSpriteInstance* backInstance = nullptr;

    // --Colour--
    std::vector<glm::vec3> colour;

    // Constructor
    EntryMenu();

    // Destructor
    ~EntryMenu();

    // Render primary rectangle
    void load_menu(uint32_t ww, uint32_t wh);
    // Ran each time cursor pos updated
    void cursor_update(double xpos, double ypos);
    // Ran each time cursor button pressed
    void cursor_click(int button);
    // Convert primary rectangle to mini ones
    void shadow_button();
    // Return mini rectangles to large ones
    void return_to_normal();
};

extern EntryMenu* MainMenu;

// Global vertdimen (vertex dimension) array to hold max vertex data
extern float vertdimen[2];

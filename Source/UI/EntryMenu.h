/*!********************************************************************************************************
\File          : EntryMenu.h
\Copyright     : GPL-3.0 License
\Brief         : Creation of entry menu when game starts and when game is paused
\Contributors  : Freddie M, Lawrence
 *********************************************************************************************************/
#pragma once

#include "Sprites.h"
#include "nlohmann/json.hpp"

class EntryMenu
{
   public:
    bool first_pass_start;
    bool first_pass_quit;
    bool start_button;
    bool pause_menu;

    bool render_start_shadow;
    bool render_quit_shadow;

    bool IS_MENU_ACTIVE;
    bool cursor_on_box;
    bool return_box_to_normal;
    double xposition;
    double yposition;

    // Textures
    // Normal start button
    const std::vector<glm::vec2> NS_tex = {{0.0f, 0.24f}, {1.0f, 0.24f}, {1.0f, 0.0f}, {0.0f, 0.0f}};
    // Depressed start button
    const std::vector<glm::vec2> DS_tex = {{0.0f, 0.49f}, {1.0f, 0.49f}, {1.0f, 0.26f}, {0.0f, 0.26f}};
    // Normal quit button
    const std::vector<glm::vec2> NQ_tex = {{0.0f, 0.74f}, {1.0f, 0.74f}, {1.0f, 0.51f}, {0.0f, 0.51f}};
    // Depressed quit button
    const std::vector<glm::vec2> DQ_tex = {{0.0f, 0.99f}, {1.0f, 0.99f}, {1.0f, 0.76f}, {0.0f, 0.76f}};

    // Initialise memeber pointers to null
    SpriteSheet* button_sprites = nullptr;

    Sprite* normal_start_button = nullptr;
    Sprite* depp_start_button = nullptr;
    Sprite* normal_quit_button = nullptr;
    Sprite* depp_quit_button = nullptr;

    UiSpriteInstance* normal_start_button_instance = nullptr;
    UiSpriteInstance* depp_start_button_instance = nullptr;
    UiSpriteInstance* normal_quit_button_instance = nullptr;
    UiSpriteInstance* depp_quit_button_instance = nullptr;

    // Intitialise locations for each box
    glm::vec3 top_button_front = {0.0f, 0.7f, 0.0f};
    glm::vec3 top_button_bottom = {0.0f, 0.7f, 0.0f};

    // Intitialise locations for each box
    glm::vec3 bottom_button_front = {0.0f, -0.7f, 0.0f};
    glm::vec3 bottom_button_bottom = {0.0f, -0.7f, 0.0f};

    // Constructor
    EntryMenu();

    // Destructor
    ~EntryMenu();

    // Enter 1 for main menu, 2 for pause initialisation
    void load_menu(int menuType);
    // Ran each time cursor pos updated
    void cursor_update(double xpos, double ypos);
    // Ran each time cursor button pressed
    void cursor_click(int button);
    // Convert render dark version of buttons
    void shadow_button();
    // Return to normal shade button
    void return_to_normal();
};

extern EntryMenu* MainMenu;

extern EntryMenu* PauseMenu;

// Global vertdimen (vertex dimension) array to hold max vertex data
extern float vertdimen[2];

// Global variable to close menu
extern bool close_window;

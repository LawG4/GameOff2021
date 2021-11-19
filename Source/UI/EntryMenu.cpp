/*!********************************************************************************************************
\File          : EntryMenu.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creation of main menu
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Sprites.h"
#include "nlohmann/json.hpp"

#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Player_object.h"
#include "collision.h"

// Constructor
EntryMenu::EntryMenu()
{
    // Colour
    colour = {{5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}, {5.0f, 1.0f, 0.0f}};

    IS_MENU_ACTIVE = false;
    cursor_on_box = false;
    return_box_to_normal = false;

    // to check if shadow button has already been ran
    first_pass = true;

    Load_side_button = false;
}

// Destructor
EntryMenu::~EntryMenu()
{
    // delete instance;
    // delete start_front;
    // delete instance2;
    // delete start_back;
}

// Generate triangles
void EntryMenu::load_menu(uint32_t ww, uint32_t wh)
{
    // Create a spritesheet
    start_front = new SpriteSheet("Textures/MenuStart.png");
    SpriteInternals::activeSheets.push_back(start_front);

    // Create a 2D triangle object
    const std::vector<glm::vec2> tex = {{0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}};
    Sprite *square = new Sprite(start_front, tex);
    instance = new SpriteInstance(square);

    // change is menu active to true
    IS_MENU_ACTIVE = true;
}

// Update button size and colour for when hovered over
void EntryMenu::shadow_button()
{
    if (first_pass) {
        Log.info("cursor in box");

        // Create a new shadow spritesheet
        SpriteSheet *start_back = new SpriteSheet("Textures/MenuStartDark.png");
        SpriteInternals::activeSheets.push_back(start_back);

        // Create a 2D triangle object
        const std::vector<glm::vec2> tex2 = {{0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}};
        Sprite *square2 = new Sprite(start_back, tex2);
        instance2 = new SpriteInstance(square2);

        // Move to front
        glm::vec3 front = {0.0f, 0.0f, 0.5f};
        instance2->setPosition(front);
        first_pass = false;

        Load_side_button = true;
    } else {
    }
}

void EntryMenu::return_to_normal()
{
    /*
    smallTriangle->isActive = false;
    smallTriangle2->isActive = false;
    Triangle->isActive = true;
    Triangle2->isActive = true;

    smallTriangle->scheduleUBOUpdate();
    smallTriangle2->scheduleUBOUpdate();
    Triangle->scheduleUBOUpdate();
    Triangle2->scheduleUBOUpdate();
    */
}

void EntryMenu::cursor_update(double xpos, double ypos)
{
    if (IS_MENU_ACTIVE) {
        // update stored cursor location
        xposition = xpos;
        yposition = ypos;
        if (collisions->check_collision(-0.5f, 0.5f, -0.5f, 0.5f, xpos, ypos) && !cursor_on_box) {
            MainMenu->shadow_button();
            return_box_to_normal = true;
        } else {
            if (return_box_to_normal) {
                MainMenu->return_to_normal();
                cursor_on_box = false;
            }
        }
    }
}

void EntryMenu::cursor_click(int button)
{
    // Left button
    if (button == 1) {
    }
    // Right button
    else if (button == 2) {
    }
}

// Declare windowdimen (window dimension) array
float vertdimen[2];

// Initialise MainMenu object
EntryMenu *MainMenu = new EntryMenu;

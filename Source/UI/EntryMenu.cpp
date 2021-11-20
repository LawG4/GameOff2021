/*!********************************************************************************************************
\File          : EntryMenu.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creation of main menu
\Contributors  : Freddie M, Lawrence G
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
    // delete the sprite instance
    // Before we delete an object first we have to ensure that it's not null
    if (frontInstance) delete frontInstance;
    if (backInstance) delete backInstance;

    // Now delete the sprites themselves
    if (startFront) delete startFront;
    if (startBack) delete startBack;

    // Delete the sprite sheets
    if (startFrontSheet) delete startFrontSheet;
    if (startBackSheet) delete startBackSheet;
}

// Generate triangles
void EntryMenu::load_menu(uint32_t ww, uint32_t wh)
{
    // Create a spritesheet
    startFrontSheet = new SpriteSheet("Textures/MenuStart.png");
    SpriteInternals::activeSheets.push_back(startFrontSheet);

    // Create a sprite, pinpointing the texture coordinates of the sprite on the sprite sheets
    const std::vector<glm::vec2> tex = {{0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}};
    startFront = new Sprite(startFrontSheet, tex);

    // Create an instance of the sprite
    frontInstance = new SpriteInstance(startFront);

    // change is menu active to true
    IS_MENU_ACTIVE = true;
}

// Update button size and colour for when hovered over
void EntryMenu::shadow_button()
{
    if (first_pass) {
        Log.info("cursor in box");

        // Create a new shadow spritesheet
        // Do these need to be seperate sheets? These could be one sheet
        startBackSheet = new SpriteSheet("Textures/MenuStartDark.png");
        SpriteInternals::activeSheets.push_back(startBackSheet);

        // Create a A sprite from the sprite sheet
        const std::vector<glm::vec2> tex2 = {{0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}};
        startBack = new Sprite(startBackSheet, tex2);

        // Create an instance of the new sprite
        backInstance = new SpriteInstance(startBack);

        // Move to front
        backInstance->setPosition(glm::vec3(0.0f, 0.0f, 0.5f));
        first_pass = false;

        Load_side_button = true;
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

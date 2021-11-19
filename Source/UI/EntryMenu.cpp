/*!********************************************************************************************************
\File          : EntryMenu.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creation of main menu
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Log.h"
#include "nlohmann/json.hpp"

#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Player_object.h"
#include "collision.h"

#include "Objects.h"

#include <fstream>
#include <iostream>

// Constructor
EntryMenu::EntryMenu()
{
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

    IS_MENU_ACTIVE = false;
    cursor_on_box = false;
    return_box_to_normal = false;

    // to check if shadow button has already been ran
    first_pass = true;
}

// Generate triangles
void EntryMenu::load_menu(uint32_t ww, uint32_t wh)
{
    // Render shapes
    Triangle = new RenderObject2D(start_largetop, colour);
    Triangle2 = new RenderObject2D(start_largebottom, colour);

    // change is menu active to true
    IS_MENU_ACTIVE = true;
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
        first_pass = false;
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

void EntryMenu::cursor_update(double xpos, double ypos)
{
    if (IS_MENU_ACTIVE) {
        if (collisions->check_collision(-1.5f, 1.5f, -0.5f, 0.5f, xpos, ypos) && !cursor_on_box) {
            MainMenu->shadow_button();
            return_box_to_normal = true;
        } else {
            if (return_box_to_normal) {
                MainMenu->return_to_normal();
                cursor_on_box = false;
                ;
            }
        }
    }
}

// Declare windowdimen (window dimension) array
float vertdimen[2];

// Initialise MainMenu object
EntryMenu *MainMenu = new EntryMenu;

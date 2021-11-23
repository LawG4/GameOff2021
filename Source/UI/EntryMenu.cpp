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
    // Variable to check if menu needs to be rendered
    IS_MENU_ACTIVE = false;
    // Cursor variables
    cursor_on_box = false;
    return_box_to_normal = false;

    // to check if shadow button has already been ran
    first_pass_start = true;
    first_pass_quit = true;

    // set logic for button shadow rendering
    render_start_shadow = false;
    render_quit_shadow = false;
}

// Destructor
EntryMenu::~EntryMenu()
{
    // delete the sprite instance
    // Before we delete an object first we have to ensure that it's not null
    if (normal_start_button_instance) delete normal_start_button_instance;
    if (depp_start_button_instance) delete depp_start_button_instance;

    if (normal_quit_button_instance) delete normal_quit_button_instance;
    if (depp_quit_button_instance) delete depp_quit_button_instance;

    // Now delete the sprites themselves
    if (normal_start_button) delete normal_start_button;
    if (depp_start_button) delete depp_start_button;

    if (normal_quit_button) delete normal_quit_button;
    if (depp_quit_button) delete depp_quit_button;

    // Delete the sprite sheets
    if (button_sprites) delete button_sprites;
}

// Generate triangles
void EntryMenu::load_menu()
{
    // Create a spritesheet
    button_sprites = new SpriteSheet("Textures/Button_sprite.png");
    SpriteInternals::activeSheets.push_back(button_sprites);

    // Create sprite's, pinpointing the texture coordinates of the sprite on the sprite sheets
    normal_start_button = new Sprite(button_sprites, NS_tex);
    normal_quit_button = new Sprite(button_sprites, NQ_tex);

    // Create an instance of the sprites
    normal_start_button_instance = new UiSpriteInstance(normal_start_button);
    normal_quit_button_instance = new UiSpriteInstance(normal_quit_button);

    // Move to correct location on screen and increase size
    normal_start_button_instance->setPosition(top_button_front);
    normal_start_button_instance->setScale(glm::vec3(2, 0.5, 0));

    normal_quit_button_instance->setPosition(bottom_button_front);
    normal_quit_button_instance->setScale(glm::vec3(2, 0.5, 0));

    // change is menu active to true
    IS_MENU_ACTIVE = true;
}

// Update button size and colour for when hovered over
void EntryMenu::shadow_button()
{
    if (start_button) {
        if (first_pass_start) {
            // Create a A sprite from the sprite sheet
            depp_start_button = new Sprite(button_sprites, DS_tex);

            // Create an instance of the new sprite
            depp_start_button_instance = new UiSpriteInstance(depp_start_button);

            // Move to front
            depp_start_button_instance->setPosition(top_button_front);
            depp_start_button_instance->setScale(glm::vec3(2, 0.5, 0));

            first_pass_start = false;

            render_start_shadow = true;
        } else {
            depp_start_button_instance->setPosition(top_button_front);
            render_start_shadow = true;
        }
    } else {
        if (first_pass_quit) {
            // Create a A sprite from the sprite sheet
            depp_quit_button = new Sprite(button_sprites, DQ_tex);

            // Create an instance of the new sprite
            depp_quit_button_instance = new UiSpriteInstance(depp_quit_button);

            // Move to front
            depp_quit_button_instance->setPosition(bottom_button_front);
            depp_quit_button_instance->setScale(glm::vec3(2, 0.5, 0));

            first_pass_quit = false;

            render_quit_shadow = true;

        } else {
            depp_quit_button_instance->setPosition(bottom_button_front);
            render_quit_shadow = true;
        }
    }
}

void EntryMenu::return_to_normal()
{
    // Turn render if statements in main windowing loop to false
    if (start_button) {
        render_start_shadow = false;
    } else {
        render_quit_shadow = false;
    }
}

void EntryMenu::cursor_update(double xpos, double ypos)
{
    if (IS_MENU_ACTIVE) {
        // update stored cursor location
        xposition = xpos;
        yposition = ypos;
        // Check collisions for start game box
        if (collisions->check_collision(-2.0f, 2.0f, -1.20f, -0.20f, xpos, ypos) && !cursor_on_box) {
            start_button = true;
            MainMenu->shadow_button();
            return_box_to_normal = true;
        }
        // Check collisions for quit game box
        else if (collisions->check_collision(-2.0f, 2.0f, 0.20f, 1.2f, xpos, ypos) && !cursor_on_box) {
            start_button = false;
            MainMenu->shadow_button();
            return_box_to_normal = true;
        }
        // If cursor on none then return everything to normal
        else {
            if (return_box_to_normal) {
                MainMenu->return_to_normal();
                cursor_on_box = false;
            }
        }
    }
}

void EntryMenu::cursor_click(int button)
{
    // Check if left cursor click happens over quit rectangle
    if (button == 0) {
        if (collisions->check_collision(-2.0f, 2.0f, 0.20f, 1.2f, xposition, yposition)) {
            close_window = true;
        }

    }
    // Right button
    else if (button == 1) {
    }
}

// Declare windowdimen (window dimension) array
float vertdimen[2];

// Initialise MainMenu object
EntryMenu *MainMenu = new EntryMenu;

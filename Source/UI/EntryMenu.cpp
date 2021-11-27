/*!********************************************************************************************************
\File          : EntryMenu.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creation of entry menu when game starts and when game is paused
\Contributors  : Freddie M, Lawrence G
 *********************************************************************************************************/

#include "Sprites.h"
#include "nlohmann/json.hpp"

#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Game_object.h"
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
void EntryMenu::load_menu(int menuType)
{
    // Create a spritesheet
    // 1 for Nornmal menu texture buttons
    if (menuType == 1) {
        button_sprites = new SpriteSheet("Textures/Button_sprite.png");
        pause_menu = false;
    }
    // 2 for pause menu button textures
    else if (menuType == 2) {
        button_sprites = new SpriteSheet("Textures/Pause_Button_sprite.png");
        pause_menu = true;
    }
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
        if (collisions->check_collision(-0.57f, 0.57f, -0.6f, -0.10f, xpos, ypos) && !cursor_on_box) {
            start_button = true;
            shadow_button();
            return_box_to_normal = true;
        }
        // Check collisions for quit game box
        else if (collisions->check_collision(-0.57f, 0.57f, 0.10f, 0.6f, xpos, ypos) && !cursor_on_box) {
            start_button = false;
            shadow_button();
            return_box_to_normal = true;
        }
        // If cursor on none then return everything to normal
        else {
            if (return_box_to_normal) {
                return_to_normal();
                cursor_on_box = false;
            }
        }
    }
}

void EntryMenu::cursor_click(int button)
{
    // Check collisions for when click happens
    if (button == 0) {
        // If quit box on main menu clicked on
        if (collisions->check_collision(-0.57f, 0.57f, 0.10f, 0.6f, xposition, yposition) && !pause_menu) {
            close_window = true;
            IS_MENU_ACTIVE = false;
        }

        // Check if Start game box has been clicked, change IS_MENU_ACTIVE to false to stop rendering this
        // menu and intialise GameObject
        else if (collisions->check_collision(-0.57f, 0.57f, -0.6f, -0.10f, xposition, yposition) &&
                 !pause_menu) {
            GameObject->Initialise();
            IS_MENU_ACTIVE = false;
            // Clear saved cursor positions
            xposition = 0;
            yposition = 0;

        }

        // If PAUSE menu open and quit button clicked return to main menu
        else if (collisions->check_collision(-0.57f, 0.57f, 0.10f, 0.6f, xposition, yposition) &&
                 pause_menu) {
            IS_MENU_ACTIVE = false;
            return_to_normal();
            MainMenu->IS_MENU_ACTIVE = true;
            MainMenu->return_to_normal();
            GameObject->start_game = false;
            // Clear saved cursor positions
            xposition = 0;
            yposition = 0;
        }

        // If resume button clicked
        else if (collisions->check_collision(-0.57f, 0.57f, -1.20f, -0.20f, xposition, yposition) &&
                 pause_menu) {
            GameObject->Initialise();
            IS_MENU_ACTIVE = false;
        }

    }
    // Right button
    else if (button == 1) {
    }
}

void EntryMenu::menu_loop(GLFWwindow *window)
{
    while (IS_MENU_ACTIVE) {
        // Initialise callbacks
        glfwSetCursorPosCallback(window, menu_cursor_position_callback);
        glfwSetMouseButtonCallback(window, menu_mouse_button_callback);

        // Start buttons
        if (render_start_shadow) {
            depp_start_button_instance->render();
        } else {
            normal_start_button_instance->render();
        }
        // Quit buttons
        if (render_quit_shadow) {
            depp_quit_button_instance->render();
        } else {
            normal_quit_button_instance->render();
        }

        // Poll GLFW for user events so they can be processed
        glfwPollEvents();
        // Use Vulkan to render the frame
        vk::drawFrame();
    }
}

// Declare windowdimen (window dimension) array
float vertdimen[2];

// Initialise MainMenu object
EntryMenu *MainMenu = new EntryMenu;

// Initialise PauseMenu object
EntryMenu *PauseMenu = new EntryMenu;

bool close_window = false;

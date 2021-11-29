/*!********************************************************************************************************
\File          : EntryMenu.cpp
\Copyright     : GPL-3.0 License
\Brief         : Creation of entry menu when game starts and when game is paused
\Contributors  : Freddie M, Lawrence G
 *********************************************************************************************************/

#include "Sprites.h"
#include "nlohmann/json.hpp"

#include "AssetHelper.h"
#include "Gameplay.h"

#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Game_object.h"
#include "Window.h"
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

    delete backgroundSprite;
    delete backgroundSheet;
    delete backgroundRightSprite;
    delete backgroundLeftSprite;
    delete backgroundSideSheet;
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

    // Load the wallpaper
    std::pair<SpriteSheet *, Sprite *> cityPair = BackgroundSprites::CityCentre();
    backgroundSheet = cityPair.first;
    backgroundSprite = cityPair.second;
    backgroundInstance = new SpriteInstance(backgroundSprite, {0, 0, 0}, {2, 2, 1}, {0, 0, 0});
    SpriteInternals::activeSheets.push_back(backgroundSheet);

    backgroundSideSheet = new SpriteSheet("Textures/CityEdges.png");
    SpriteInternals::activeSheets.push_back(backgroundSideSheet);
    backgroundLeftSprite =
      new Sprite(backgroundSideSheet, Textures::generateTexCoordinates({0, 0}, {168, 512}, {512, 512}));
    backgroundRightSprite =
      new Sprite(backgroundSideSheet, Textures::generateTexCoordinates({168, 0}, {168, 512}, {512, 512}));

    // Get how wide the frame is, so we can make sure the whole screen is covered
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    windowSize(width, height);
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
            // depp_quit_button_instance->setPosition(bottom_button_front);
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

glm::vec2 normaliseCursorCoordinates(glm::vec2 mousePos)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glm::vec2 screenspace;

    // Normalise x coordinate,
    screenspace.x = 2.0 * (mousePos.x / width) - 1.0;
    screenspace.y = 1.0 - ((mousePos.y / height) * 2.0);

    return screenspace;
}

void EntryMenu::cursor_update(double xpos, double ypos)
{
    if (IS_MENU_ACTIVE) {
        // update stored cursor location

        glm::vec2 mouseCoord = normaliseCursorCoordinates({xpos, ypos});

        // Check collisions for start game box
        if (Collision::pointInBox(mouseCoord, {{-0.57f, 0.6f}, 1.15f, 0.5f}) && !cursor_on_box) {
            start_button = true;
            shadow_button();
            return_box_to_normal = true;
            top_button = true;

        }
        // Check collisions for quit game box
        else if (Collision::pointInBox(mouseCoord, {{-0.57f, -0.1f}, 1.15f, 0.5f}) && !cursor_on_box) {
            start_button = false;
            shadow_button();
            return_box_to_normal = true;
            top_button = false;
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
        if (!top_button && !pause_menu) {
            close_window = true;
            IS_MENU_ACTIVE = false;
            top_button = false;
        }

        // Check if Start game box has been clicked, change IS_MENU_ACTIVE to false to stop rendering this
        // menu and intialise GameObject
        else if (top_button && !pause_menu) {
            game_state = true;
            IS_MENU_ACTIVE = false;
            top_button = false;
        }

        // If PAUSE menu open and quit button clicked return to main menu
        else if (!top_button && pause_menu) {
            IS_MENU_ACTIVE = false;
            return_to_normal();
            MainMenu->IS_MENU_ACTIVE = true;
            MainMenu->return_to_normal();
            game_state = false;
        }

        // If resume button clicked
        else if (top_button && pause_menu) {
            IS_MENU_ACTIVE = false;
        }

    }
    // Right button
    else if (button == 1) {
    }
}

void EntryMenu::menu_loop(GLFWwindow *window)
{  // Just pause menu now
    // Initialise callbacks
    glfwSetCursorPosCallback(window, menu_cursor_position_callback);
    glfwSetMouseButtonCallback(window, menu_mouse_button_callback);

    while (IS_MENU_ACTIVE) {
        // Check if X button in top left has been clicked
        if (glfwWindowShouldClose(window)) {
            break;
        }

        // First the background
        backgroundInstance->render();

        for (SpriteInstance &sprite : backgroundSides) {
            sprite.render();
        }

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

void EntryMenu::windowSize(uint32_t width, uint32_t height)
{
    // We know that the window is 2 units height. So how many units wide is it?

    float uniformWidth = 2 * static_cast<float>(width) / static_cast<float>(height);
    // backgroundInstance->setScale({uniformWidth, 2.0, 1.0});
    Log.info("Window is {} units wide", uniformWidth);

    // empty the sides vector
    backgroundSides.clear();

    // How wide is the side slice anyway?
    glm::vec3 sideSize = Textures::getTexSize({168, 512});

    // How many side slices are rquired on each side
    uint32_t sideCount = glm::ceil(uniformWidth / (2.0 * sideSize.x));

    // We can use an array to keep each track of which one to add currently
    Sprite *sides[2] = {backgroundLeftSprite, backgroundRightSprite};
    uint32_t sideTracker = 1;
    for (uint32_t i = 0; i < sideCount; i++) {
        // Which sprite are we putting on each side?
        SpriteInstance leftInstance = SpriteInstance(sides[sideTracker]);
        SpriteInstance rightInstance = SpriteInstance(sides[sideTracker ^ 1]);

        // Now place each sprite instance appropriatly
        leftInstance.setPosition({-1.0f - 1.5 * i * sideSize.x, 0, 0});
        rightInstance.setPosition({1.0f + 1.5 * i * sideSize.x, 0, 0});

        // Pass on the scales of the instances
        leftInstance.setScale(sideSize);
        rightInstance.setScale(sideSize);

        // Track them
        backgroundSides.push_back(leftInstance);
        backgroundSides.push_back(rightInstance);

        // Invert our trakcer
        sideTracker ^= 1;
    }
}

// Initialise MainMenu object
EntryMenu *MainMenu = new EntryMenu;

// Initialise PauseMenu object
EntryMenu *PauseMenu = new EntryMenu;

bool close_window = false;

/*!********************************************************************************************************
\File          : Main.cpp
\Copyright     : GPL-3.0 License
\Brief         : Adds an entry point into the game
\Contributors  : Lawrence G, Freddie M
 *********************************************************************************************************/
#include "Log.h"
#include "Window.h"
#include "nlohmann/json.hpp"

#include "Camera.h"
#include "Cursor_input.h"
#include "EntryMenu.h"
#include "Game_object.h"
#include "Keyboard_input.h"
#include "collision.h"

#include "Gameplay.h"
#include "Sounds.h"
#include "Sprites.h"
#include "Timer.h"

#include <fstream>
#include <iostream>

/// <summary> Destory everything </summary>
void cleanUp()
{
    Gameplay::cleanup();

    delete MainMenu;
    delete PauseMenu;

    // First destroy Vulkan
    cleanupVulkan();

    // Destroy player object
    delete player_class;

    // Destroy music engine when music no longer needed
    Sounds::stopmusic();

    // If the window exists destroy it
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
    Log.info("GLFW resources destroyed");

    Log.info("All resources destroyed");
}

int main(int argc, char* argv[])
{
    if (!initLog()) {
        std::cout << "Somehow the logger failed to init!\nWhat on earth happened??" << std::endl;
        return -1;
    }

    // Read in the user settings Json File
    std::ifstream userSettingFile("UserSettings.json");
    if (!userSettingFile) {
        Log.warn("Could not open user settings json file");
    } else {
        Log.info("Opened user settings json file");
    }

    // Parse the settings file into the json object
    nlohmann::json userSetting;
    userSettingFile >> userSetting;
    userSettingFile.close();

    // Set the defaults for the window
    bool fullScreen = true;
    uint32_t windowWidth = 720;
    uint32_t windowHeight = 400;

    // Used to set up cursor coordinate array
    bool settingsOverride = userSetting["Override_Default_Settings"];
    if (settingsOverride) {
        Log.info("Json file detected that the user wants to override default settings");
        fullScreen = userSetting["Window_Settings"]["FullScreen"];
        windowWidth = userSetting["Window_Settings"]["Windowed_Width"];
        windowHeight = userSetting["Window_Settings"]["Windowed_Height"];
    }

    // Create the window with the settings
    if (!initWindow("Chronicals of Barry : Why did we make a buggy game engine from scratch, originally this "
                    "was a critisism of crypto, but we didn't have time so now you just collect doge coins",
                    windowWidth, windowHeight, fullScreen)) {
        Log.error("Could not initialise GLFW window for the laucher");
        cleanUp();
        return -1;
    }

    // Ensure that the machine is capable of loading Vulkan at all
    if (!glfwVulkanSupported()) {
        Log.error("Vulkan is not supported on your device");
        cleanUp();
        return -1;
    } else {
        Log.info("GLFW Confirms a Vulkan device");
    }

    // Initialise vulkan
    if (!initVulkan()) {
        Log.error("Could not start Vulkan");
        cleanUp();
        return -1;
    }

    // Key input data
    glfwSetKeyCallback(window, key_callback);

    // Set camera position to the centre
    Camera::setPosition({0, 0, 0});

    // Load MainMenu, enter 1 for main menu, 2 for pause
    MainMenu->load_menu(1);
    PauseMenu->load_menu(2);

    // Run MainMenu first
    MainMenu->IS_MENU_ACTIVE = true;
    // Sound check
    bool soundcheck = false;
    // Set the resize callback
    glfwSetWindowSizeCallback(window, Camera::onWindowSize);

    // Initialise callbacks to main menu first thing
    glfwSetCursorPosCallback(window, menu_cursor_position_callback);
    glfwSetMouseButtonCallback(window, menu_mouse_button_callback);

    // Enter into the windowing loop
    while (!glfwWindowShouldClose(window)) {
        // Start this frames clock, so we can know how long it took
        Time::startFrameTime();

        // Poll GLFW for user events so they can be processed
        glfwPollEvents();

        // Use Vulkan to render the frame
        vk::drawFrame();

        // Check if menu is active, if it, enter menu loop
        if (MainMenu->IS_MENU_ACTIVE) {
            // First the background
            MainMenu->backgroundInstance->render();
            for (SpriteInstance& sprite : MainMenu->backgroundSides) {
                sprite.render();
            }

            // Start buttons
            if (MainMenu->render_start_shadow) {
                MainMenu->depp_start_button_instance->render();
            } else {
                MainMenu->normal_start_button_instance->render();
            }
            // Quit buttons
            if (MainMenu->render_quit_shadow) {
                MainMenu->depp_quit_button_instance->render();
            } else {
                MainMenu->normal_quit_button_instance->render();
            }
        }

        // Check to see if game loop needs to be entered
        if (game_state) {
            Log.info("Starting Game");
            if (!soundcheck) {
                // Play music
                Sounds::music();
                soundcheck = true;
            }
            // Initialise game and assets
            Gameplay::initialise();
            // Enter gameLoop
            Gameplay::gameLoop();
        }

        // If the user has asked the window to close through the Ui then schedule window destruction
        if (close_window == true) {
            glfwSetWindowShouldClose(window, true);
        }

        // Frame has finished so end the clock so we know how long it took
        Time::EndFrameTime();
    }

    cleanUp();
}

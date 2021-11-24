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
#include "Sprites.h"
#include "Timer.h"

#include <fstream>
#include <iostream>

/// <summary> Destory everything </summary>
void cleanUp()
{
    delete MainMenu;
    delete PauseMenu;

    // First destroy Vulkan
    cleanupVulkan();

    // Destroy player object
    delete player_class;

    // If the window exists destroy it
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
    Log.info("GLFW resources destroyed");

    Log.info("All resources destroyed");
}

int main(int argc, char *argv[])
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

    // Input vertex data
    vertdimen[0] = 3.5f;
    vertdimen[1] = 2.0f;

    // Used to set up cursor coordinate array
    bool settingsOverride = userSetting["Override_Default_Settings"];
    if (settingsOverride) {
        Log.info("Json file detected that the user wants to override default settings");
        fullScreen = userSetting["Window_Settings"]["FullScreen"];
        windowWidth = userSetting["Window_Settings"]["Windowed_Width"];
        windowHeight = userSetting["Window_Settings"]["Windowed_Height"];
    }

    // Create the window with the settings
    if (!initWindow("B.U.G Launcher", windowWidth, windowHeight, fullScreen)) {
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

    // Mouse input positon data
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Mouse input button click data
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Set camera position to the centre
    Camera::setPosition({0, 0, 0});

    // Load MainMenu, enter 1 for main menu, 2 for pause
    MainMenu->load_menu(1);
    PauseMenu->load_menu(2);
    collisions->intialise_object(windowWidth, windowHeight);

    // Run MainMenu first
    MainMenu->IS_MENU_ACTIVE = true;

    // Enter into the windowing loop
    while (!glfwWindowShouldClose(window)) {
        // Start this frames clock, so we can know how long it took
        Time::startFrameTime();

        // Poll GLFW for user events so they can be processed
        glfwPollEvents();

        // Check if menu is active, if it, render buttons
        if (MainMenu->IS_MENU_ACTIVE) {
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
        // Same block of if's as above but for pause screen
        if (PauseMenu->IS_MENU_ACTIVE) {
            // Start buttons
            if (PauseMenu->render_start_shadow) {
                PauseMenu->depp_start_button_instance->render();
            } else {
                PauseMenu->normal_start_button_instance->render();
            }
            // Quit buttons
            if (PauseMenu->render_quit_shadow) {
                PauseMenu->depp_quit_button_instance->render();
            } else {
                PauseMenu->normal_quit_button_instance->render();
            }
        }

        // If the user has asked the window to close through the Ui then schedule window destruction
        if (close_window == true) {
            glfwSetWindowShouldClose(window, true);
        }

        // Is the gameplay loop running
        if (Gameplay::isActive()) {
            // Run the frame and pass the delta time to the game
            Gameplay::playFrame(Time::getDetlaTime());
        } else {
            // Check the main menu to see if the initalisation has been clicked
            if (GameObject->start_game) {
                Log.info("Starting Game");
                Gameplay::initialise();
            }
        }

        // Use Vulkan to render the frame
        vk::drawFrame();

        // Frame has finished so end the clock so we know how long it took
        Time::EndFrameTime();
    }

    cleanUp();
}

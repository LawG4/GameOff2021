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
#include "Keyboard_input.h"
#include "Player_object.h"
#include "collision.h"

#include "Sprites.h"

#include <fstream>
#include <iostream>

/// <summary> Destory everything </summary>
void cleanUp()
{
    delete MainMenu;

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

    // Input vertex data
    vertdimen[0] = 3.5f;
    vertdimen[1] = 2.0f;

    // Set camera position to the centre
    Camera::setPosition({0, 0, 0});

    // Load newly declared values into MainMenu
    MainMenu->load_menu();
    collisions->intialise_object(windowWidth, windowHeight);

    // Paramater for swaying the camera back and forth
    float t = 0;

    // A regular object that interacts with the camera
    SpriteInstance worldObject(MainMenu->startFront);
    worldObject.setPosition({0, 0, 0});

    // Enter into the windowing loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (MainMenu->Load_side_button) {  // Check if shadow sprite is ready to be rendered
            MainMenu->backInstance->render();
        }
        if (MainMenu->IS_MENU_ACTIVE) {  // Check if menu is active, if it, render its frames
            // MainMenu->frontInstance->setPosition({0, 0, 0});
            MainMenu->frontInstance->render();
        }

        // worldObject.render();

        // Camera::setPosition(glm::vec3(cos(t), 0, 0));
        Camera::setPosition(glm::vec3(0, 1, 0));
        t += 0.01f;

        if (MainMenu->close_window == true) {
            glfwSetWindowShouldClose(window, true);
        }

        vk::drawFrame();
    }

    cleanUp();
}

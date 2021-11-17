/*!********************************************************************************************************
\File          : Main.cpp
\Copyright     : GPL-3.0 License
\Brief         : Adds an entry point into the game
\Contributors  : Lawrence G, Freddie M
 *********************************************************************************************************/
#include "Log.h"
#include "Window.h"
#include "nlohmann/json.hpp"

#include "Cursor_input.h"
#include "Keyboard_input.h"
#include "Player_object.h"

#include "Sprites.h"

#include <fstream>
#include <iostream>

/// <summary> Destory everything </summary>
void cleanUp()
{
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

    // Create a spritesheet
    SpriteSheet *sheet = new SpriteSheet("Cum.png");

    // Create a 2D triangle object
    const std::vector<glm::vec3> pos = {{0.0f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}};
    const std::vector<glm::vec3> col = {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
    Sprite *Triangle = new Sprite(pos, col);
    Sprite *Triangle2 = new Sprite(pos, col);
    float time = 0;

    // Enter into the windowing loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Key input data
        glfwSetKeyCallback(window, key_callback);

        // Mouse input data
        glfwSetCursorPosCallback(window, cursor_position_callback);

        Triangle->rot += glm::vec3(0.0, 0.1, 0.0f);
        Triangle->scheduleUBOUpdate();

        Triangle2->pos = glm::vec3(3 * cos(time), 0, 0);
        Triangle2->scheduleUBOUpdate();
        time += 0.1f;

        vk::drawFrame();
    }

    delete (sheet);
    cleanUp();
}

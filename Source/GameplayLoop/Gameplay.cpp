/*!********************************************************************************************************
\File          : Gameplay.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G, Freddie M
 *********************************************************************************************************/

#include "Gameplay.h"
#include "Animation.h"
#include "AssetHelper.h"
#include "EntryMenu.h"
#include "Log.h"
#include "Timer.h"
#include "Window.h"

bool _init = false;
bool _isActive = false;
bool Gameplay::isActive() { return _isActive; }

// Store the grasshopper and coordinates
SpriteSheet* _coinSheet = nullptr;
AnimatedSprite* _coin = nullptr;

void gameplay_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Store the hopper position
    glm::vec3 pos = _coin->getPosition();

    // Get the state for the D button for foward
    int state = glfwGetKey(window, GLFW_KEY_D);
    if (state == GLFW_PRESS) {
        // move the hopper to the right
        // Ideally this will be set the acceleration because we don't have delta time in the callback
        _coin->setPosition(pos + glm::vec3(0.1, 0.0, 0.0));
    }

    // Escape to pause menu
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // Call pause menu
        PauseMenu->IS_MENU_ACTIVE = true;
    }
}

void Gameplay::initialise()
{
    // Tell the gameplay loop that we can actually render something next time
    _isActive = true;
    _init = true;

    std::pair<SpriteSheet*, AnimatedSprite*> coin = AnimatedSprites::spinningCoin();
    _coinSheet = coin.first;
    SpriteInternals::activeSheets.push_back(_coinSheet);
    _coin = coin.second;

    // Tell GLFW that we're now using the gameplay key callback
    glfwSetKeyCallback(window, gameplay_key_callback);
}

void Gameplay::playFrame(float deltaTime)
{
    //_hopper->setRotation(_hopper->getRotation() + deltaTime * glm::vec3(0, 0, 1));
    _coin->updateDelta(deltaTime);
    _coin->render();
}

void Gameplay::cleanup()
{
    // If we haven't initalised then there's nothing to cleanup
    if (!_init) return;
    _init = false;

    // release the rest of the resources
    Log.info("releasing gameplay loop resources");

    delete _coin;
    delete _coinSheet;
}

void Gameplay::gameLoop()
{
    while (_isActive) {
        // Start this frames clock, so we can know how long it took
        Time::startFrameTime();

        // Poll GLFW for user events so they can be processed
        glfwPollEvents();

        // Run the frame and pass the delta time to the game
        Gameplay::playFrame(Time::getDetlaTime());

        // Use Vulkan to render the frame
        vk::drawFrame();

        // Check if pause menu is active, if it, enter pause menu loop
        if (PauseMenu->IS_MENU_ACTIVE) {
            _isActive = PauseMenu->menu_loop(window);
        }
        // Frame has finished so end the clock so we know how long it took
        Time::EndFrameTime();
    }
}

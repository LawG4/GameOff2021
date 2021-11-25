/*!********************************************************************************************************
\File          : Gameplay.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Gameplay.h"
#include "Log.h"
#include "Sprites.h"
#include "Window.h"

bool _init = false;
bool _isActive = false;
bool Gameplay::isActive() { return _isActive; }

// Store the grasshopper and coordinates
SpriteSheet* _hopperSheet = nullptr;
Sprite* _hopperSprite = nullptr;
SpriteInstance* _hopper = nullptr;

void gameplay_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Store the hopper position
    glm::vec3 pos = _hopper->getPosition();

    // Get the state for the D button for foward
    int state = glfwGetKey(window, GLFW_KEY_D);
    if (state == GLFW_PRESS) {
        // move the hopper to the right
        // Ideally this will be set the acceleration because we don't have delta time in the callback
        _hopper->setPosition(pos + glm::vec3(0.1, 0.0, 0.0));
    }
}

void Gameplay::initialise()
{
    // Tell the gameplay loop that we can actually render something next time
    _isActive = true;
    _init = true;

    // Load up the grass hopper
    _hopperSheet = new SpriteSheet("Textures/TempHopper.png");
    SpriteInternals::activeSheets.push_back(_hopperSheet);
    _hopperSprite = new Sprite(_hopperSheet, {{0, 1}, {1, 1}, {1, 0}, {0, 0}});
    _hopper = new SpriteInstance(_hopperSprite);

    // Tell GLFW that we're now using the gameplay key callback
    glfwSetKeyCallback(window, gameplay_key_callback);
}

void Gameplay::playFrame(float deltaTime)
{
    //_hopper->setRotation(_hopper->getRotation() + deltaTime * glm::vec3(0, 0, 1));
    _hopper->render();
}

void Gameplay::cleanup()
{
    // If we haven't initalised then there's nothing to cleanup
    if (!_init) return;
    _init = false;

    // release the rest of the resources
    Log.info("releasing gameplay loop resources");

    delete _hopper;
    delete _hopperSprite;
    delete _hopperSheet;
}

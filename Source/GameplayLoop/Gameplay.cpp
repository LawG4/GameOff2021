/*!********************************************************************************************************
\File          : Gameplay.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G, Freddie M
 *********************************************************************************************************/

#include "Game_object.h"

#include "Animation.h"
#include "AssetHelper.h"
#include "EntryMenu.h"
#include "Gameplay.h"
#include "Log.h"
#include "Timer.h"
#include "Window.h"
#include "collision.h"

bool _init = false;
bool _isActive = false;
bool Gameplay::isActive() { return _isActive; }

// Store the assets and coordinates
SpriteSheet* _coinSheet = nullptr;
AnimatedSprite* _coin = nullptr;

SpriteSheet* _hopperSheetwalk = nullptr;
AnimatedSprite* _walkhopper = nullptr;
AnimationInstance walkInstnace;

SpriteSheet* _hopperSheetjump = nullptr;
AnimatedSprite* _jumphopper = nullptr;

void gameplay_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // If a key has been released
    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_D:  // D key released
                Physics::setHorizontalAcceleration(0);
                break;
            case GLFW_KEY_A:  // A key released
                Physics::setHorizontalAcceleration(0);
                break;
            default:
                break;
        }
    }

    // If a key has been pressed
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_SPACE:  // Space key pressed
                // Make the hopper jump
                Physics::jump();
                break;
            case GLFW_KEY_D:  // D Key pressed
                Physics::setHorizontalAcceleration(1.0);
                break;
            case GLFW_KEY_A:  // A Key pressed
                Physics::setHorizontalAcceleration(-1.0);
                break;
            case GLFW_KEY_ESCAPE:  // Escape key pressed
                // Make the pause menu show
                PauseMenu->IS_MENU_ACTIVE = true;
                break;
            default:
                break;
        }
    }
}

SpriteSheet* backgroundSheet;
SpriteSheet* backgroundSideSheet;
Sprite* backgroundSprite;
Sprite* backgroundLeftSprite;
Sprite* backgroundRightSprite;
SpriteInstance* backgroundInstance;
std::vector<SpriteInstance> backgroundSides;

void Gameplay::initialise()
{
    // Tell the gameplay loop that we can actually render something next time
    _isActive = true;
    _init = true;

    // Load the assets

    // Coin
    std::pair<SpriteSheet*, AnimatedSprite*> coin = AnimatedSprites::spinningCoin();
    _coinSheet = coin.first;
    SpriteInternals::activeSheets.push_back(_coinSheet);
    _coin = coin.second;
    _coin->setPosition({-3.0, 0, 0});

    // Walking hopper
    std::pair<SpriteSheet*, AnimatedSprite*> walkhopper = AnimatedSprites::hopperwalk();
    _hopperSheetwalk = walkhopper.first;
    SpriteInternals::activeSheets.push_back(_hopperSheetwalk);
    _walkhopper = walkhopper.second;
    walkInstnace = AnimationInstance(_walkhopper);
    _walkhopper->setPosition({0, 0, 0});

    // Jumping hoppper
    std::pair<SpriteSheet*, AnimatedSprite*> jumphopper = AnimatedSprites::hopperjump();
    _hopperSheetjump = jumphopper.first;
    SpriteInternals::activeSheets.push_back(_hopperSheetjump);
    _jumphopper = jumphopper.second;
    _jumphopper->setPosition({1.0, 0, 0});

    // Load the wallpaper
    std::pair<SpriteSheet*, Sprite*> cityPair = BackgroundSprites::CityCentre();
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
    Gameplay::windowSize(width, height);

    // Tell GLFW that we're now using the gameplay key callback
    glfwSetKeyCallback(window, gameplay_key_callback);
}

void Gameplay::playFrame(float deltaTime)
{
    _coin->updateDelta(deltaTime);
    _coin->render();

    // Update the hoppers position using the physics engine
    _walkhopper->setPosition(glm::vec3(Physics::updatePosition(deltaTime, {}), _walkhopper->getPosition().z));

    // Update the hoppers animation
    _walkhopper->updateDelta(Physics::getVelocity().x * deltaTime);

    // Finally render the hopper
    _walkhopper->render();
    walkInstnace.render();

    _jumphopper->updateDelta(deltaTime);
    _jumphopper->render();

    backgroundInstance->render();
    for (SpriteInstance& sprite : backgroundSides) {
        sprite.render();
    }
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
    delete _walkhopper;
    delete _hopperSheetwalk;
    delete _jumphopper;
    delete _hopperSheetjump;
    delete backgroundSprite;
    delete backgroundSheet;
    delete backgroundRightSprite;
    delete backgroundLeftSprite;
    delete backgroundSideSheet;
}

void Gameplay::gameLoop()
{
    while (_isActive) {
        // Start this frames clock, so we can know how long it took
        Time::startFrameTime();

        // Poll GLFW for user events so they can be processed
        glfwPollEvents();

        // Check if pause menu is active, if it, enter pause menu loop
        if (PauseMenu->IS_MENU_ACTIVE) {
            PauseMenu->menu_loop(window);
        }

        // If the window x button was pressed then break out
        if (glfwWindowShouldClose(window)) {
            _isActive = false;
            Gameplay::cleanup();
            break;
        }

        // If user has chosen to quit game, break out and return to main menu loop
        if (!game_state) {
            MainMenu->IS_MENU_ACTIVE = true;
            game_state = false;
            _isActive = false;
            // Gameplay::cleanup();
            break;
        }

        // Run the frame and pass the delta time to the game
        Gameplay::playFrame(Time::getDetlaTime());

        // Use Vulkan to render the frame
        vk::drawFrame();

        // Frame has finished so end the clock so we know how long it took
        Time::EndFrameTime();
    }
}

void Gameplay::windowSize(uint32_t width, uint32_t height)
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
    Sprite* sides[2] = {backgroundLeftSprite, backgroundRightSprite};
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

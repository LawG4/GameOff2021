/*!********************************************************************************************************
\File          : Gameplay.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G, Freddie M
 *********************************************************************************************************/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "Animation.h"
#include "AssetHelper.h"
#include "Camera.h"
#include "EntryMenu.h"
#include "Game_object.h"
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

// Floor tiles
SpriteSheet* floor_sheet;
Sprite* floor_sprite;
SpriteInstance* floor_instance;

// server1 tiles
SpriteSheet* _server1_sheet;
Sprite* _server1_sprite;

// server2 tiles
SpriteSheet* _server2_sheet;
Sprite* _server2_sprite;

// server3 tiles
SpriteSheet* _server3_sheet;
Sprite* _server3_sprite;

// server4 tiles
SpriteSheet* _server4_sheet;
Sprite* _server4_sprite;

SpriteInstance* floorarray[40];
SpriteInstance* serverarray[10];
SpriteInstance* render_array[10];

// Global variable for storing position
std::vector<SpriteInstance> Infinite_vector_list_thing;
float hoppborder;
std::vector<int> location_directions(80);
glm::vec3 world_vect_limit = {0, -0.8, 0};

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

    // Seed random with current time
    std::srand(std::time(nullptr));

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

    // Load floor
    std::pair<SpriteSheet*, Sprite*> floorpair = BackgroundSprites::floor();
    floor_sheet = floorpair.first;
    SpriteInternals::activeSheets.push_back(floor_sheet);
    floor_sprite = floorpair.second;

    float tempfloat = 0;
    for (int i = 0; i < 40; i++) {
        tempfloat += 0.29295;
        SpriteInstance* floor = new SpriteInstance(floor_sprite, {tempfloat - 6, -1.1, 0},
                                                   Textures::getTexSize({75, 75}), {0, 0, 0});
        floorarray[i] = floor;
    }

    // Load server 1
    std::pair<SpriteSheet*, Sprite*> server1pair = BackgroundSprites::server1();
    _server1_sheet = server1pair.first;
    SpriteInternals::activeSheets.push_back(_server1_sheet);
    _server1_sprite = server1pair.second;
    serverarray[0] =
      new SpriteInstance(_server1_sprite, {0, 0, 0}, Textures::getTexSize({75, 75}), {0, 0, 0});

    // Load server 2
    std::pair<SpriteSheet*, Sprite*> server2pair = BackgroundSprites::server2();
    _server2_sheet = server2pair.first;
    SpriteInternals::activeSheets.push_back(_server2_sheet);
    _server2_sprite = server2pair.second;
    serverarray[1] =
      new SpriteInstance(_server2_sprite, {0, 0, 0}, Textures::getTexSize({75, 75}), {0, 0, 0});

    // Load server 3
    std::pair<SpriteSheet*, Sprite*> server3pair = BackgroundSprites::server3();
    _server3_sheet = server3pair.first;
    SpriteInternals::activeSheets.push_back(_server3_sheet);
    _server3_sprite = server3pair.second;
    serverarray[2] =
      new SpriteInstance(_server3_sprite, {0, 0, 0}, Textures::getTexSize({75, 75}), {0, 0, 0});

    // Load server 4
    std::pair<SpriteSheet*, Sprite*> server4pair = BackgroundSprites::server4();
    _server4_sheet = server4pair.first;
    SpriteInternals::activeSheets.push_back(_server4_sheet);
    _server4_sprite = server4pair.second;
    serverarray[3] =
      new SpriteInstance(_server4_sprite, {0, 0, 0}, Textures::getTexSize({75, 75}), {0, 0, 0});

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

    // Something to decide number of spawns
    glm::vec3 nextposition;
    glm::vec3 postion = Camera::getPosition();
    hoppborder = postion[0];

    // If character has moved "out of view (view is 10x8)"
    if (world_vect_limit[0] < postion[0]) {
        // Nested for loop (first loop 10 times) (nexted: loop 8 times)
        Gameplay::randWallValue();
        int counter = 0;
        for (int row = 0; row < 10; row++) {
            // Add 1 to move accross x axis
            world_vect_limit[0] += 0.3;
            nextposition = world_vect_limit;

            for (int i = 0; i < 8; i++) {
                // If choice 1 then copy server 1 instance
                if (location_directions[counter] == 1) {
                    SpriteInstance temp = SpriteInstance(_server1_sprite, nextposition,
                                                         Textures::getTexSize({75, 75}), {0, 0, 0});
                    Infinite_vector_list_thing.push_back(temp);

                    nextposition[1] += 0.29295;

                    // If choice 2 then copy server 2 instance
                } else if (location_directions[counter] == 2) {
                    SpriteInstance temp = SpriteInstance(_server2_sprite, nextposition,
                                                         Textures::getTexSize({75, 75}), {0, 0, 0});
                    Infinite_vector_list_thing.push_back(temp);

                    nextposition[1] += 0.29295;

                }

                else if (location_directions[counter] == 3) {
                    SpriteInstance temp = SpriteInstance(_server3_sprite, nextposition,
                                                         Textures::getTexSize({75, 75}), {0, 0, 0});
                    Infinite_vector_list_thing.push_back(temp);

                    nextposition[1] += 0.29295;
                }

                else if (location_directions[counter] == 4) {
                    SpriteInstance temp = SpriteInstance(_server4_sprite, nextposition,
                                                         Textures::getTexSize({75, 75}), {0, 0, 0});
                    Infinite_vector_list_thing.push_back(temp);

                    nextposition[1] += 0.29295;
                }

                else if (location_directions[counter] == 0) {
                    nextposition[1] += 0.29295;

                } else {
                }
                counter += 1;
            }
        }
        // At the end of nested for loops add 10 to world_vect_limit
        world_vect_limit[0] += 3;
    }

    for (SpriteInstance x : Infinite_vector_list_thing) {
        x.render();
    }

    // Update the hoppers animation
    _walkhopper->updateDelta(Physics::getVelocity().x * deltaTime);

    // Finally render the hopper
    _walkhopper->render();
    walkInstnace.render();

    _jumphopper->updateDelta(deltaTime);
    _jumphopper->render();

    for (int i = 0; i < 40; i++) {
        floorarray[i]->render();
    }

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
            _isActive = false;
            Gameplay::randWallValue();
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
    uint32_t sideTracker = 0;
    for (uint32_t i = 0; i < sideCount; i++) {
        // Which sprite are we putting on each side?
        SpriteInstance leftInstance = SpriteInstance(sides[sideTracker]);
        SpriteInstance rightInstance = SpriteInstance(sides[sideTracker ^ 1]);

        // Now place each sprite instance appropriatly
        leftInstance.setPosition({-1.33f - i * sideSize.x, 0, 0});
        rightInstance.setPosition({1.32f + i * sideSize.x, 0, 0});

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

// Function to create array that will hold instructions for what next set of blocks will be
void Gameplay::randWallValue()
{
    // if (location_directions[0] != NULL) location_directions.clear();

    // Loop to create our numbers and based on them decide fate of blocks
    for (int i = 0; i < 80; i++) {
        float random_variable = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        if (random_variable > 0.50) {
            if (random_variable > 0.50 && random_variable < 0.60) {
                location_directions[i] = 1;
            } else if (random_variable > 0.60 && random_variable < 0.70) {
                location_directions[i] = 2;
            } else if (random_variable > 0.80 && random_variable < 0.90) {
                location_directions[i] = 3;
            } else if (random_variable > 0.90 && random_variable < 1.0) {
                location_directions[i] = 4;
            } else {
                location_directions[i] = 0;
            }
        } else {
            location_directions[i] = 0;
        }
    }
}

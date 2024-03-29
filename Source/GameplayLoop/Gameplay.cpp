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
#include "Score.h"
#include "Timer.h"
#include "Window.h"
#include "collision.h"

bool _init = false;
bool _isActive = false;
bool Gameplay::isActive() { return _isActive; }

const glm::vec2 _spriteSize = glm::vec2(0.28, 0.10);
const glm::vec2 _spriteOffset = glm::vec2(-0.16, -0.04);

// Store the assets and coordinates
SpriteSheet* _coinSheet = nullptr;
AnimatedSprite* _coin = nullptr;
AnimationInstance _coinInstance;

SpriteSheet* _hopperSheetwalk = nullptr;
AnimatedSprite* _walkhopper = nullptr;
AnimationInstance walkInstnace;

SpriteSheet* _hopperSheetjump = nullptr;
AnimatedSprite* _jumphopper = nullptr;

// Floor tiles
SpriteSheet* floor_sheet;
Sprite* floor_sprite;
std::vector<SpriteInstance> floorInstances;

namespace Keys
{
bool A = false;
bool D = false;
bool SPACE = false;
}  // namespace Keys

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
std::vector<AnimationInstance> coin_vector;
float hoppborder;
std::vector<int> location_directions(240);
glm::vec3 world_vect_limit = {0, -0.8, 0};
std::vector<int> coing_vector(240);

void gameplay_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // If a key has been pressed
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_SPACE:  // Space key pressed
                Keys::SPACE = true;
                break;
            case GLFW_KEY_D:  // D Key pressed
                Keys::D = true;
                break;
            case GLFW_KEY_A:  // A Key pressed
                Keys::A = true;
                break;
            case GLFW_KEY_ESCAPE:  // Escape key pressed
                // Make the pause menu show
                PauseMenu->IS_MENU_ACTIVE = true;
                break;
            default:
                break;
        }
    }
    // Has a key been released?
    else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_SPACE:  // Space key released
                Keys::SPACE = false;
                break;
            case GLFW_KEY_D:  // D Key released
                Keys::D = false;
                break;
            case GLFW_KEY_A:  // A Key released
                Keys::A = false;
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
UiSpriteInstance* backgroundInstance;
std::vector<UiSpriteInstance> backgroundSides;

float floorPannelWidth;
float _uniformWidth;

std::vector<BoundingRect> PhysicsBoxes;
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
    _coinInstance = AnimationInstance(_coin);
    _coinInstance.setScale({0.1, 0.1, 1.0});
    _coinInstance.setPosition({0.4, -0.5, 0});

    // Use the coin to init the score
    Score::init(_coin);

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
    backgroundInstance = new UiSpriteInstance(backgroundSprite);
    backgroundInstance->setPosition({0, 0, 0.9});
    backgroundInstance->setScale({2, 2, 1});
    SpriteInternals::activeSheets.push_back(backgroundSheet);

    backgroundSideSheet = new SpriteSheet("Textures/CityEdges.png");
    SpriteInternals::activeSheets.push_back(backgroundSideSheet);
    backgroundLeftSprite =
      new Sprite(backgroundSideSheet, Textures::generateTexCoordinates({0, 0}, {168, 512}, {512, 512}));
    backgroundRightSprite =
      new Sprite(backgroundSideSheet, Textures::generateTexCoordinates({168, 0}, {168, 512}, {512, 512}));

    // Use a physics box
    PhysicsBoxes.push_back(Physics::boxFromSprite(_coinInstance));

    // Get how wide the frame is, so we can make sure the whole screen is covered
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    Gameplay::windowSize(width, height);

    // Tell GLFW that we're now using the gameplay key callback
    glfwSetKeyCallback(window, gameplay_key_callback);
}

void Gameplay::playFrame(float deltaTime)
{
    // Move the camera
    Camera::scroll(deltaTime);

    // Move the floor if it has gone out of range
    glm::vec2 leftCorner = {Camera::getPosition().x - 0.5f * _uniformWidth,
                            Camera::getPosition().y - 0.5f * 2.0};
    while (floorInstances[0].getPosition().x < leftCorner.x - floorPannelWidth / 2.0f) {
        // Pop the first floor tile off
        floorInstances.erase(floorInstances.begin());

        // Create a new sprite instance based on the back
        SpriteInstance temp = floorInstances[floorInstances.size() - 1];
        temp.setPosition(temp.getPosition() + glm::vec3(floorPannelWidth, 0, 0));

        // Add this new one to the list
        floorInstances.push_back(temp);
    }

    // When the player is off screen
    if (_walkhopper->getPosition().x < leftCorner.x) {
        Log.info("Dead");
        exit(1);
    }

    // Update the velocity based on the user's key input
    if (Keys::SPACE) Physics::jump();

    // No left or right keys are being pressed, or both are being pressed
    if (!(Keys::A || Keys::D) || (Keys::A && Keys::D)) {
        Physics::setHorizontalAcceleration(0);
        // Only left key is pressed
    } else if (Keys::A) {
        // Apply a faster tern around
        if (Physics::getVelocity().x > 0) {
            Physics::setHorizontalAcceleration(-8);
        } else {
            Physics::setHorizontalAcceleration(-3);
        }
        // Only right key is pressed
    } else if (Keys::D) {
        // Apply a faster tern around
        if (Physics::getVelocity().x > 0) {
            Physics::setHorizontalAcceleration(3);
        } else {
            Physics::setHorizontalAcceleration(8);
        }
    }

    // Update the hoppers position using the physics engine
    _walkhopper->setPosition(
      glm::vec3(Physics::updatePosition(deltaTime, PhysicsBoxes), _walkhopper->getPosition().z));

    // Something to decide number of spawns
    glm::vec3 nextposition;
    glm::vec3 postion = Camera::getPosition();
    hoppborder = postion[0];

    // If character has moved "out of view (view is 10x8)"
    if (world_vect_limit[0] < postion[0] + 6) {
        // Nested for loop (first loop 10 times) (nexted: loop 8 times)
        Gameplay::randWallValue();
        int counter = 0;
        for (int row = 0; row < 30; row++) {
            // Add 1 to move accross x axis
            world_vect_limit[0] += 0.29295;
            nextposition = world_vect_limit;

            for (int i = 0; i < 8; i++) {
                // Coin instances
                if (coing_vector[counter] == 1) {
                    AnimationInstance temp =
                      AnimationInstance(_coin, nextposition, {0.1, 0.1, 0.1}, {0, 0, 0});
                    coin_vector.push_back(temp);
                }
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
        world_vect_limit[0] += 0;
    }

    // Physics for server boxes
    for (SpriteInstance x : Infinite_vector_list_thing) {
        x.render();
        // Use a physics box
        PhysicsBoxes.push_back(Physics::boxFromSprite(x));
    }

    BoundingRect spriteRect{
      glm::vec2(_walkhopper->getPosition().x, _walkhopper->getPosition().y) + _spriteOffset, _spriteSize.x,
      _spriteSize.y};
    // Physics for coins
    for (uint32_t i = 0; i < coin_vector.size(); i++) {
        coin_vector[i].render();
        if (Collision::boxInBox(spriteRect, Physics::boxFromSprite(coin_vector[i]))) {
            // We've touched a coin
            Score::score++;
            coin_vector[i].setPosition(glm::vec3(0.0f, 0.0f, -100));
        }
    }

    // Update the hoppers animation
    _walkhopper->updateDelta(Physics::getVelocity().x * deltaTime);

    // Now make the hopper face the right direction
    if (Physics::getVelocity().x < 0) {
        _walkhopper->setRotation({0, glm::pi<float>(), 0});
        glm::vec3 pos = _walkhopper->getPosition();
        pos.z = 0.4;
        _walkhopper->setPosition(pos);

    } else {
        _walkhopper->setRotation({0, 0, 0});
        glm::vec3 pos = _walkhopper->getPosition();
        pos.z = 0.0;
        _walkhopper->setPosition(pos);
    }

    // Finally render the hopper
    _walkhopper->render();

    _coin->updateDelta(deltaTime);
    //_coinInstance.render();

    // walkInstnace.render();

    // _jumphopper->updateDelta(deltaTime);
    // _jumphopper->render();

    for (SpriteInstance& sprite : floorInstances) {
        sprite.render();
    }

    backgroundInstance->render();
    for (SpriteInstance& sprite : backgroundSides) {
        sprite.render();
    }

    // render the score
    Score::parseScore(_uniformWidth);
    Score::render();
}

void Gameplay::cleanup()
{
    // If we haven't initalised then there's nothing to cleanup
    if (!_init) return;
    _init = false;

    // release the rest of the resources
    Log.info("releasing gameplay loop resources");

    // cleanup score renderer
    Score::deinit();

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
    delete floor_sprite;
    delete floor_sheet;
    delete _server1_sprite;
    delete _server2_sprite;
    delete _server3_sprite;
    delete _server4_sprite;
    delete _server4_sheet;
    delete _server3_sheet;
    delete _server2_sheet;
    delete _server1_sheet;
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
    _uniformWidth = uniformWidth;
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
        UiSpriteInstance leftInstance = UiSpriteInstance(sides[sideTracker]);
        UiSpriteInstance rightInstance = UiSpriteInstance(sides[sideTracker ^ 1]);

        // Now place each sprite instance appropriatly
        leftInstance.setPosition({-1.0f - 1.5 * i * sideSize.x, 0, 0.9});
        rightInstance.setPosition({1.0f + 1.5 * i * sideSize.x, 0, 0.9});

        // Pass on the scales of the instances
        leftInstance.setScale(sideSize);
        rightInstance.setScale(sideSize);

        // Track them
        backgroundSides.push_back(leftInstance);
        backgroundSides.push_back(rightInstance);

        // Invert our trakcer
        sideTracker ^= 1;
    }

    // Now calculate how many floor tiles are needed to cover the screen
    floorInstances.clear();
    sideSize = Textures::getTexSize({75, 75});
    floorPannelWidth = sideSize.x;
    sideCount = glm::ceil(uniformWidth / sideSize.x) + 1;

    // Calculate the world position of the bottom left corner
    glm::vec2 corner = {Camera::getPosition().x - 0.5f * uniformWidth, Camera::getPosition().y - 0.5f * 2.0};

    // Place them
    floorInstances.resize(sideCount);
    for (uint32_t i = 0; i < sideCount; i++) {
        floorInstances[i] = SpriteInstance(
          floor_sprite, glm::vec3(corner + glm::vec2(i * sideSize.x, 0), -0.8), sideSize, glm::vec3(0.0f));
    }
}

// Function to create array that will hold instructions for what next set of blocks will be
void Gameplay::randWallValue()
{
    // if (location_directions[0] != NULL) location_directions.clear();

    // Loop to create our numbers and based on them decide fate of blocks
    for (int i = 0; i < 240; i++) {
        float random_variable = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        if (random_variable > 0.58) {
            if (i == 0) {
            }

            else if (location_directions[i - 1] == 0) {
                if (random_variable > 0.55 && random_variable < 0.58) {
                    location_directions[i] = 1;
                    coing_vector[i] = 0;
                } else if (random_variable > 0.68 && random_variable < 0.70) {
                    location_directions[i] = 2;
                    coing_vector[i] = 0;
                } else if (random_variable > 0.80 && random_variable < 0.85) {
                    location_directions[i] = 3;
                    coing_vector[i] = 0;
                } else if (random_variable > 0.86 && random_variable < 0.9) {
                    location_directions[i] = 4;
                    coing_vector[i] = 0;
                } else if (random_variable > 0.9 && random_variable < 1) {
                    coing_vector[i] = 1;
                } else {
                    location_directions[i] = 0;
                    coing_vector[i] = 0;
                }
            } else {
                location_directions[i] = 0;
                coing_vector[i] = 0;
            }
        }
    }
}

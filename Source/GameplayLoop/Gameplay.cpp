/*!********************************************************************************************************
\File          : Gameplay.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Gameplay.h"
#include "Log.h"
#include "Sprites.h"

bool _init = false;
bool _isActive = false;
bool Gameplay::isActive() { return _isActive; }

// Store the grasshopper and coordinates
SpriteSheet* _hopperSheet = nullptr;
Sprite* _hopperSprite = nullptr;
SpriteInstance* _hopper = nullptr;

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
}

void Gameplay::playFrame(float deltaTime) { _hopper->render(); }

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

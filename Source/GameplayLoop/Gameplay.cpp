/*!********************************************************************************************************
\File          : Gameplay.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Gameplay.h"
#include "Log.h"

bool _init = false;

bool _isActive = false;
bool Gameplay::isActive() { return _isActive; }

void Gameplay::initialise()
{
    // Tell the gameplay loop that we can actually render something next time
    _isActive = true;
    _init = true;
}

void Gameplay::playFrame(float deltaTime) {}

void Gameplay::cleanup()
{
    // If we haven't initalised then there's nothing to cleanup
    if (!_init) return;
    _init = false;

    // release the rest of the resources
    Log.info("releasing gameplay loop resources");
}

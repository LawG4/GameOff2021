/*!********************************************************************************************************
\File          : Gameplay.cpp
\Copyright     : GPL-3.0 License
\Brief         : Provides the gameplay loop so that main doesn't get too clustered
\Contributors  : Lawrence G,
 *********************************************************************************************************/

#include "Gameplay.h"

bool _isActive = false;
bool Gameplay::isActive() { return _isActive; }

void Gameplay::initialise()
{
    // Tell the gameplay loop that we can actually render something next time
    _isActive = true;
}

void Gameplay::playFrame(float deltaTime) {}

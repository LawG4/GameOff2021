/*!********************************************************************************************************
\File          : Score.h
\Copyright     : GPL-3.0 License
\Brief         : Provides score tracking
\Contributors  : Lawrence G,
 *********************************************************************************************************/
#pragma once
#include "Animation.h"
#include "Camera.h"
#include "Gameplay.h"

namespace Score
{

extern uint32_t score;
void render();

void parseScore(float uniformWidth);

void init(AnimatedSprite* coinPointer);

void deinit();
}  // namespace Score

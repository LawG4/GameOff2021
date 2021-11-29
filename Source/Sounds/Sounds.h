/*!********************************************************************************************************
\File          : Sounds.h
\Copyright     : GPL-3.0 License
\Brief         : Sounds fu
 *********************************************************************************************************/
#pragma once

// Just test if sound works
#include <irrKlang.h>

irrklang::ISoundEngine* engine;
namespace Sounds
{
// Fucntion to play main sound tract
void music();
}  // namespace Sounds

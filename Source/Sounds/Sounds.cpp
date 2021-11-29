/*!********************************************************************************************************
\File          : Sounds.cpp
\Copyright     : GPL-3.0 License
\Brief         : Sounds functions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Sounds.h"

void Sounds::music()
{
    // Testing start a sound engine
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    engine->play2D("Sounds/Chill.mp3");
}

/*!********************************************************************************************************
\File          : Sounds.cpp
\Copyright     : GPL-3.0 License
\Brief         : Sounds functions
\Contributors  : Freddie M
 *********************************************************************************************************/

#include "Sounds.h"

irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

void Sounds::music()
{
    // Testing start a sound engine
    engine->play2D("Sounds/Chill.mp3", true);
}

void Sounds::stopmusic() { engine->drop(); }

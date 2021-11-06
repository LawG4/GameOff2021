/*!********************************************************************************************************
\File          : Player_object.h
\Copyright     : MIT License
\Brief         : Define player object
\Contributors  : Freddie M,
 *********************************************************************************************************/
#pragma once #
#include "Player.h"

class player
{  // Player class to hold player data and basic player functions
   public:
    int player_health;
    int player_damage;
    int player_step_size;
};

// deffine player object
// player build_player();
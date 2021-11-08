/*!********************************************************************************************************
\File          : Player_object.h
\Copyright     : GPL-3.0 License
\Brief         : Define player object
\Contributors  : Freddie M,
 *********************************************************************************************************/
#pragma once

class player
{  // Player class to hold player data and basic player functions
   public:
    int player_health;
    int player_damage;
    int player_step_size;

    player();
    ~player();
    int out_player_damage();
};

extern player* player_class;

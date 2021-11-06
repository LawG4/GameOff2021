/*!********************************************************************************************************
\File          : Player_object.cpp
\Copyright     : MIT License
\Brief         : Player object functions
\Contributors  : Freddie M,
 *********************************************************************************************************/

#include "Player_object.h"
#include <iostream>
#include "Log.h"

/*
=== Plan ===

Make player class/struct object thing
- 3 parts (Physical[movements],Actions[attacks, maybe merge with physical??],General updates[e.g health
updates, all all player data e.g attack pwoer ect])

- Detect when playable, so when paused or un paused
- Take key commands and

*/

class player
{  // Player class to hold player data and basic player functions
   public:
    int player_health;
    int player_damage;
    int player_step_size;

    player()
    {  // Player constructor to create base player data values
        player_health = 100;
        player_damage = 5;
        player_step_size = 1;
    }

    ~player()
    {  // Message for when destructor called
        Log.info("Player object destroyed");
    }
};

player* build_player()
{  // Function to create player object
    return new player();
}

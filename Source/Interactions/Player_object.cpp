/*!********************************************************************************************************
\File          : Player_object.cpp
\Copyright     : MIT License
\Brief         : Player object functions
\Contributors  : Freddie M,
 ********************************************************************************************************/

#include "Player_object.h"
#include <iostream>
#include "Player.h"

/*
=== Plan ===

Make player class/struct object thing
- 3 parts (Physical[movements],Actions[attacks, maybe merge with physical??],General updates[e.g health
updates, all all player data e.g attack pwoer ect])

- Detect when playable, so when paused or un paused
- Take key commands and

*/

// Player class methods
player::player()
{  // Player constructor to create base player data values
    player_health = 100;
    player_damage = 5;
    player_step_size = 1;
    player_step_size = 1;
};

player::~player()
{  // Message for when destructor called
    Log.info("Player object destroyed");
};

int player::out_player_damage()
{  // Print out player damage
    return player_damage;
};

// Build player class as GLOBAL
player* player_class = new player;

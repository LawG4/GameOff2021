/*!********************************************************************************************************
\File          : Game_object.cpp
\Copyright     : GPL-3.0 License
\Brief         : Player object functions
\Contributors  : Freddie M,
 ********************************************************************************************************/

#include "Game_object.h"
#include <iostream>
#include "Keyboard_input.h"

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

Game_object::Game_object() { start_game = false; }

void Game_object::Initialise() { start_game = true; }

// Build GameObject class as GLOBAL
Game_object* GameObject = new Game_object;

// Global variable
bool game_state = false;

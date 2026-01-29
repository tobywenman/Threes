#pragma once

#include "grid.h"
#include <stdlib.h>

#define hand_size 3
#define max_players 16
#define bag_size 3*3*3*3 // 27 unique tiles with 3 copies each

// A players hand
typedef struct hand_t
{
    tile_t tiles[hand_size];
    size_t num_tiles;
}hand_t;

// Tiles a player is playing along with their positions
typedef struct turn_t
{
    tile_t tiles[hand_size];
    pos_t poses[hand_size];
    size_t num_tiles;
}turn_t;

typedef struct player_t
{
    hand_t hand;
    unsigned score;
}player_t;

typedef struct game_state_t
{
    grid_t* grid;

    player_t players[max_players];
    size_t num_players;

    // tiles available in the bag
    tile_t bag[bag_size];
    size_t num_tiles;
}game_state_t;

game_state_t* init_game();

void fill_bag(tile_t* bag);

bool verify_legal(const grid_t* grid, pos_t pos, tile_t tile);
bool verify_legal_line(const grid_t* grid, pos_t pos, tile_t tile, bool horizontal);
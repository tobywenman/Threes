#pragma once

#include "grid.h"
#include "client_main.h"

#define tile_path "../src/client/img/tiles.bmp"

#define tile_size 16

typedef struct game_main_data_t
{
    char* server_addr;
    grid_t* grid;

    SDL_Surface* tiles;

    size_t x_offset;
    size_t y_offset;
}game_main_data_t;

void game_main_init(main_state_t* state, char* server_addr);
bool game_main(main_state_t* state);

void blit_tile(SDL_Surface* tiles, tile_t tile, SDL_Surface* surface, size_t tile_x, size_t tile_y);
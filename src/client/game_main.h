#pragma once

#include "game.h"
#include "client_main.h"
#include "button.h"

#define tile_path "../src/client/img/tiles.bmp"

#define tile_size 16

#define hand_x 4
#define hand_y 4

#define tile_border 1
#define hand_border 1

typedef struct game_main_data_t
{
    char* server_addr;
    grid_t* grid;

    SDL_Surface* tiles;

    float x_offset;
    float y_offset;

    button_manager_t hand_buttons;

    hand_t player_hand;
}game_main_data_t;

void game_main_init(main_state_t* state, char* server_addr);
bool game_main(main_state_t* state);

void draw_grid(main_state_t* state);

void blit_tile(SDL_Surface* tiles, tile_t tile, SDL_Surface* surface, size_t tile_x, size_t tile_y);

void draw_hand(main_state_t* state);
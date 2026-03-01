#pragma once

#include "client_main.h"
#include "button.h"
#include "numbers.h"
#include "networking.h"

#include <SDL3_ttf/SDL_ttf.h>

#define max_input_chars 1000

typedef struct lobby_data_t
{
    size_t num_players;

    TTF_Font* font;

    button_manager_t button_manager;
    button_id_t ready_id;

    SDL_Surface* numbers;

    network_data_t* network_data;
}lobby_data_t;

void lobby_init(main_state_t* state, network_data_t* network_data);
bool lobby_main(main_state_t* state);
void lobby_destroy(main_state_t* state);
#pragma once

#include "client_main.h"

#include <SDL3_ttf/SDL_ttf.h>

typedef struct server_join_data_t
{
    char* input_string;
    size_t string_length;
    TTF_Font* font;
}server_join_data_t;

void server_join_init(main_state_t* state);
bool server_join_main(main_state_t* state);
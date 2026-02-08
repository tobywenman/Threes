#pragma once

#include "client_main.h"

#include <SDL3_ttf/SDL_ttf.h>

#define max_input_chars 1000

typedef struct server_join_data_t
{
    char input_string[max_input_chars];
    size_t string_length;
    TTF_Font* font;
}server_join_data_t;

void server_join_init(main_state_t* state);
bool server_join_main(main_state_t* state);
void server_join_destroy(main_state_t* state);
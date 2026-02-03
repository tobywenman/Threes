#pragma once

#include "client_main.h"

#include <SDL3_ttf/SDL_ttf.h>

#define font_path "../src/client/font/font.ttf"

typedef struct splash_state_t
{
    TTF_Font* font;
}splash_state_t;

void splash_init(main_state_t* state);
bool splash_main(main_state_t* state);
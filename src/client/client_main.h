#pragma once

#include <SDL3/SDL.h>

#include "states.h"

#define px_width 320
#define px_height 180

typedef struct main_state_t
{
    SDL_Window* win;
    SDL_Surface* win_surface;
    SDL_Surface* draw_surface;

    state_e state;

    void* state_data;

    float width_mult;
    float height_mult;
}main_state_t;

bool init_main_state(main_state_t* state);

void destroy_free_state(main_state_t* state);
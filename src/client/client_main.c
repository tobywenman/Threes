#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_pixels.h>

#include <stdlib.h>
#include <stdio.h>

#include "client_main.h"
#include "splash.h"

#define fps_cap 60
#define frame_ns 1000000000 / fps_cap

int main()
{
    main_state_t* state = malloc(sizeof(main_state_t));;
    if (!init_main_state(state))
        return 1;

    uint64_t frame_start_time;

    while(call_state_main(state))
    {
        frame_start_time = SDL_GetTicksNS();
        SDL_BlitSurfaceScaled(state->draw_surface, NULL, state->win_surface, NULL, SDL_SCALEMODE_PIXELART);
        SDL_UpdateWindowSurface(state->win);

        uint64_t time_diff = SDL_GetTicksNS() - frame_start_time;
        if (time_diff < frame_ns)
        {
            SDL_DelayNS(frame_ns - time_diff);
        }
    }
    destroy_free_state(state);
}

bool init_main_state(main_state_t* state)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        goto init_error;

    state->win = SDL_CreateWindow("Threees!", 640, 480, 0);
    if (state->win == NULL)
        goto init_error;

    state->draw_surface = SDL_CreateSurface(px_width, px_height, SDL_PIXELFORMAT_RGB24);
    if (state->draw_surface == NULL)
        goto init_error;

    state->win_surface = SDL_GetWindowSurface(state->win);
    if (state->win_surface == NULL)
        goto init_error;

    state->state = SPLASH;

    state->width_mult = (float)state->draw_surface->w / (float)state->win_surface->w;
    state->height_mult = (float)state->draw_surface->h / (float)state->win_surface->h;

    splash_init(state);

    return true;

    init_error:
        fprintf(stderr, "%s\n", SDL_GetError());
        return false;
}

void destroy_free_state(main_state_t* state)
{
    SDL_DestroyWindow(state->win);
    SDL_DestroySurface(state->draw_surface);
    SDL_Quit();
    free(state);
}
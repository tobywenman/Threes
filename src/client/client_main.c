#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_pixels.h>

#include <stdlib.h>
#include <stdio.h>

#include "client_main.h"
#include "splash.h"

int main()
{
    main_state_t* state = malloc(sizeof(main_state_t));;
    if (!init_main_state(state))
        return 1;

    while(call_state_main(state))
    {
        SDL_BlitSurfaceScaled(state->draw_surface, NULL, state->win_surface, NULL, SDL_SCALEMODE_PIXELART);
        SDL_UpdateWindowSurface(state->win);
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
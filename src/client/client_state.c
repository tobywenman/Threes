#include "client_state.h"

#include <stdio.h>
#include <stdlib.h>

client_state_t* init_state()
{
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    client_state_t* state = malloc(sizeof(client_state_t));

    state->cur_scene = SPLASH;

    state->win = SDL_CreateWindow("Threees!", 640,480, 0);
    if (state->win == NULL)
    {
        fprintf(stderr, "window couldn't be made! %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    state->renderer = SDL_CreateRenderer(state->win, NULL);
    if (state->renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(state->win);
        SDL_Quit();
        return NULL;
    }

    splash_init(state);

    return state;
}

void destroy_state(client_state_t* state)
{
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->win);
    SDL_Quit();
}

void change_scene(client_state_t* state, scene_e new_scene)
{
    switch (state->cur_scene)
    {
        case SPLASH:
            splash_end(state);
            break;
        // case MENU:
        //     menu_end();
        //     break;
        // case SERVER_JOIN:
        //     server_join_end();
        //     break;
        // case GAME:
        //     game_end();
        //     break;
    }

    switch (new_scene)
    {
        case SPLASH:
            splash_init(state);
            break;
        // case MENU:
        //     menu_init();
        //     break;
        // case SERVER_JOIN:
        //     server_join_init();
        //     break;
        // case GAME:
        //     game_init();
        //     break;
    }

    state->cur_scene = new_scene;
}

bool call_scene_func(client_state_t* state)
{
    switch (state->cur_scene)
    {
        case SPLASH:
            splash_main(state);
            return true;
        break;
        // case MENU:
        // break;
        // case SERVER_JOIN:
        // break;
        // case GAME:
        // break;
        case QUIT:
            quit_func(state);
            return false;
        break;
    }
}

void splash_init(client_state_t* state)
{
    splash_state_t* scene_state = malloc(sizeof(splash_state_t));

    SDL_Surface* bmp = SDL_LoadBMP("../src/client/img/box.bmp");
    if (bmp == NULL) {
        fprintf(stderr, "SDL_LoadBMP Error: %s\n", SDL_GetError());
        SDL_Quit();
    }
    scene_state->background_texture = SDL_CreateTextureFromSurface(state->renderer, bmp);
    SDL_DestroySurface(bmp);

    TTF_Font* font = TTF_OpenFont("../src/client/img/Freedom-10eM.ttf", 28 );
    SDL_Color textcolour = {0x00, 0x00, 0x00, 0xFF};
    
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, "textureText.c_str()", 0, textcolour);

    scene_state->text_texture = SDL_CreateTextureFromSurface(state->renderer, text_surface);
    SDL_DestroySurface(text_surface);

    state->scene_state = scene_state;
}

void splash_main(client_state_t* state)
{
    SDL_Event e;
    SDL_zero(e);
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            change_scene(state, QUIT);
        }
    }
    SDL_RenderClear(state->renderer);
    SDL_RenderTexture(state->renderer, ((splash_state_t*)(state->scene_state))->background_texture, NULL, NULL);

    SDL_FRect texture_size;
    SDL_GetTextureSize(((splash_state_t*)(state->scene_state))->text_texture, &texture_size.w, &texture_size.h);
    SDL_RenderTexture(state->renderer, ((splash_state_t*)(state->scene_state))->text_texture, NULL, &texture_size);

    SDL_RenderPresent(state->renderer);
}

void splash_end(client_state_t* state)
{
    SDL_DestroyTexture(((splash_state_t*)(state->scene_state))->background_texture);
    free(state->scene_state);
}


void quit_func(client_state_t* state)
{
    destroy_state(state);
}
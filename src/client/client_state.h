#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef enum scene_e
{
    SPLASH,
    MENU,
    SERVER_JOIN,
    GAME,
    QUIT,
}scene_e;

typedef struct client_state_t
{
    SDL_Window* win;
    SDL_Renderer* renderer;

    scene_e cur_scene;
    void* scene_state;
}client_state_t;

// call the appropriate game scene
bool call_scene_func(client_state_t* state);

void change_scene(client_state_t* state, scene_e new_scene);

typedef struct splash_state_t
{
    SDL_Texture* background_texture;
    SDL_Texture* text_texture;
}splash_state_t;

// Each scene has a init, main loop and destructor function
void splash_init(client_state_t* state);
void splash_main(client_state_t* state);
void splash_end(client_state_t* state);


void quit_func(client_state_t* state);


client_state_t* init_state();

void destroy_state(client_state_t* state);
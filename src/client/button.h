#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>

typedef struct button_t
{
    SDL_Rect size_pos;

    SDL_Surface* text;
    uint32_t norm_colour;
    uint32_t hover_colour;

    bool hovering;
}button_t;

typedef struct button_manager_t
{
    button_t* buttons;
    size_t container_size;
    size_t num_buttons;
    TTF_Font* font;
}button_manager_t;

typedef size_t button_id_t;

button_manager_t init_button_manager();

button_id_t add_button(button_manager_t* manager, SDL_Rect size_pos, char* text);

bool get_id_at_pos(const button_manager_t* manager, button_id_t* id, float x, float y);

void update_hover_mouse(button_manager_t* manager, float x, float y);

void blit_buttons(const button_manager_t* manager, SDL_Surface* target);

void destroy_buttons(button_manager_t* manager);
#include "game_main.h"

#include <SDL3_image/SDL_image.h>

void game_main_init(main_state_t* state, char* server_addr)
{
    game_main_data_t* data = malloc(sizeof(game_main_data_t));

    data->server_addr = server_addr;
    data->grid = init_grid();

    data->tiles = IMG_Load(tile_path);

    data->x_offset = 0;
    data->y_offset = 0;

    state->state_data = data;

    state->state = GAME;

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
}

bool game_main(main_state_t* state)
{
    game_main_data_t* data = state->state_data;
    SDL_Event e;
    SDL_zero(e);

    //Get event data
    while(SDL_PollEvent(&e) == true)
    {
        //If event is quit type
        if(e.type == SDL_EVENT_QUIT)
        {
            //End the main loop
            return false;
        }
        if (e.type == SDL_EVENT_MOUSE_MOTION)
        {
            if (e.motion.state & SDL_BUTTON_RMASK)
            {
                data->x_offset += e.motion.xrel * get_x_scale_factor(state);
                data->y_offset += e.motion.yrel * get_y_scale_factor(state);
            }
        }
    }

    draw_grid(state);

    blit_tile(data->tiles, generate_tile(0,0,0), state->draw_surface, 0, 0);
    return true;
}

void blit_tile(SDL_Surface* tiles, tile_t tile, SDL_Surface* surface, size_t tile_x, size_t tile_y)
{
    SDL_Rect target = {tile_x,tile_y,tile_size,tile_size};
    if (!tile_valid)
    {
        SDL_FillSurfaceRect(surface, &target, 0xFFFFFF);
    }
    else
    {
        uint8_t colour = tile_colour(tile);
        uint8_t count = tile_count(tile);
        uint8_t shape = tile_shape(tile);
    
        SDL_Rect src_rect = {count*tile_size, (colour*tile_size*3) + (shape*tile_size), tile_size, tile_size};
    
        SDL_BlitSurface(tiles, &src_rect, surface, &target);
    }
}

void draw_grid(main_state_t* state)
{
    game_main_data_t* data = state->state_data;

    int x_offset = (int)data->x_offset % tile_size;
    int y_offset = (int)data->y_offset % tile_size;

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
    for (int i=x_offset; i<state->draw_surface->w; i+=tile_size)
    {
        SDL_Rect line_rect = {i, 0, 1, state->draw_surface->h};
        SDL_FillSurfaceRect(state->draw_surface, &line_rect, 0);
    }
    for (int i=y_offset; i<state->draw_surface->h; i+=tile_size)
    {
        SDL_Rect line_rect = {0, i, state->draw_surface->w, 1};
        SDL_FillSurfaceRect(state->draw_surface, &line_rect, 0);
    }
}

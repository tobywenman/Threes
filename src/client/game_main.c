#include "game_main.h"

#include <SDL3_image/SDL_image.h>


void game_main_init(main_state_t* state, char* server_addr)
{
    game_main_data_t* data = malloc(sizeof(game_main_data_t));

    data->server_addr = server_addr;
    data->grid = init_grid();

    data->tiles = IMG_Load(tile_path);

    state->state_data = data;

    state->state = GAME;

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);

    int x_offset = 0;
    int y_offset = 0;
    for (uint8_t colour=0; colour<3; colour++)
    {
        for (uint8_t count=0; count<3; count++)
        {
            for (uint8_t shape=0; shape<3; shape++)
            {
                tile_t tile = generate_tile(colour, count, shape);
                blit_tile(data->tiles, tile, state->draw_surface, x_offset*tile_size, y_offset*tile_size);
                ++x_offset;
            }
        }
        x_offset = 0;
        ++y_offset;
    }
}

bool game_main(main_state_t* state)
{
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
    }
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
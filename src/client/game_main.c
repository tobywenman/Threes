#include "game_main.h"

#include <stdio.h>
#include <SDL3_image/SDL_image.h>

static float clamp_offset(size_t min, size_t max, size_t screen_width, float offset)
{
    float min_limit = (max+2)*tile_size-screen_width;
    float max_limit = (min+1)*tile_size;

    if (offset < 0)
        return 0;
    else if (offset < min_limit)
        return min_limit;
    else if (offset > max_limit)
        return max_limit;
    else
        return offset;
}

static void limit_offsets(main_state_t* state)
{
    game_main_data_t* data = state->state_data;

    size_t min_x, max_x, min_y, max_y;

    #define border_width 2

    if (find_corners(data->grid, &min_x, &max_x, &min_y, &max_y))
    {
        data->x_offset = clamp_offset(min_x, max_x, px_width, data->x_offset);
        data->y_offset = clamp_offset(min_y, max_y, px_height, data->y_offset);
    }
    else
    {
        data->x_offset = 0;
        data->y_offset = 0;
    }
}

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

    pos_t pos = {32,32};
    tile_t tile = generate_tile(0,0,0);

    set_tile(data->grid, pos, tile, true);
    pos.x += 1;
    tile = generate_tile(0,0,1);
    set_tile(data->grid, pos, tile, false);
    pos.x += 1;
    tile = generate_tile(0,0,2);
    set_tile(data->grid, pos, tile, false);
    pos.x += 1;
    tile = generate_tile(1,0,2);
    set_tile(data->grid, pos, tile, false);

    limit_offsets(state);
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
                data->x_offset -= e.motion.xrel * get_x_scale_factor(state);
                data->y_offset -= e.motion.yrel * get_y_scale_factor(state);

                limit_offsets(state);
            }
        }
    }

    draw_grid(state);

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

    size_t x_shift = tile_size - ((size_t)(data->x_offset) % tile_size);
    size_t y_shift = tile_size - ((size_t)(data->y_offset) % tile_size);

    size_t x_num_tiles = (px_width / tile_size);
    size_t y_num_tiles = (px_height / tile_size);

    size_t x_tile_start = ((size_t)(data->x_offset) / tile_size);
    size_t y_tile_start = ((size_t)(data->y_offset) / tile_size);

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
    for (size_t i=x_shift; i<state->draw_surface->w; i+=tile_size)
    {
        SDL_Rect line_rect = {i, 0, 1, state->draw_surface->h};
        SDL_FillSurfaceRect(state->draw_surface, &line_rect, 0);
    }
    for (size_t i=y_shift; i<state->draw_surface->h; i+=tile_size)
    {
        SDL_Rect line_rect = {0, i, state->draw_surface->w, 1};
        SDL_FillSurfaceRect(state->draw_surface, &line_rect, 0);
    }

    for (size_t x = x_tile_start-1; x<x_tile_start+x_num_tiles; x++)
    {
        for (size_t y = y_tile_start-1; y<y_tile_start+y_num_tiles+1; y++)
        {
            pos_t pos = {x, y};
            tile_t tile = read_tile(data->grid, pos);
            if (tile_valid(tile))
                blit_tile(data->tiles, tile, state->draw_surface, ((x-x_tile_start)*tile_size)+x_shift, ((y-y_tile_start)*tile_size)+y_shift);
        }
    }
}

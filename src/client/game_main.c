#include "game_main.h"

#include <stdio.h>
#include <SDL3_image/SDL_image.h>

static float clamp_offset(size_t min, size_t max, size_t screen_width, float offset)
{
    float min_limit = (min+2)*tile_size-screen_width;
    float max_limit = (max+1)*tile_size;

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
        data->x_offset = (grid_size/2)*tile_size;
        data->y_offset = (grid_size/2)*tile_size;
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

    limit_offsets(state);

    data->hand_buttons = init_button_manager();

    {
        SDL_Rect button_rect = {hand_x+tile_border,hand_y+tile_border,tile_size+2*tile_border, tile_size + tile_border*2};
        for (size_t i=0; i<hand_size; i++)
        {
            SDL_Surface* butt_surface = SDL_CreateSurface(button_rect.w, button_rect.h, SDL_PIXELFORMAT_RGBA8888);
            add_button_surface(&data->hand_buttons, button_rect, butt_surface);
            button_rect.y += tile_size + tile_border*2;
        }
    }

    SDL_Rect finish_turn_rect = {hand_x, px_height-hand_y-20, 50, 20};

    add_button_text(&data->hand_buttons, finish_turn_rect, "Place");

    data->player_hand.num_tiles = hand_size;
    for (size_t i=0; i<hand_size; i++) 
        data->player_hand.tiles[i] = generate_tile(i, 0, 0);

    data->first_tile = true;
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
            update_hover_mouse(&data->hand_buttons, e.motion.x*state->width_mult, e.motion.y*state->height_mult);
        }
        if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (e.button.button == 1)
            {
                button_id_t id;
                if (get_id_at_pos(&data->hand_buttons, &id, e.button.x*state->width_mult, e.button.y*state->height_mult))
                {
                    if (id < hand_size)
                    {
                        select_button(&data->hand_buttons, id);
                    }
                }
                else
                {
                    if (get_selected(&data->hand_buttons, &id))
                    {
                        tile_t tile = data->player_hand.tiles[id];

                        pos_t pos = {((e.motion.x*state->width_mult + data->x_offset)/tile_size)-1, ((e.button.y*state->height_mult + data->y_offset)/tile_size)-1};

                        set_tile(data->grid, pos, tile, data->first_tile);
                        data->first_tile = false;
                    }
                }
            }
        }
    }

    draw_grid(state);

    draw_hand(state);

    return true;
}

void blit_tile(SDL_Surface* tiles, tile_t tile, SDL_Surface* surface, size_t tile_x, size_t tile_y)
{
    SDL_Rect target = {tile_x,tile_y,tile_size,tile_size};
    if (!tile_valid(tile))
    {
        SDL_FillSurfaceRect(surface, &target, 0);
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

void draw_hand(main_state_t* state)
{
    game_main_data_t* data = state->state_data;

    size_t width = tile_size + tile_border*2 + hand_border*2;
    size_t height = (tile_size + 2*tile_border)*(hand_size) + hand_border*2;

    SDL_Rect rect = {hand_x, hand_y, width, height};

    SDL_FillSurfaceRect(state->draw_surface, &rect, 0xFFFFFF);

    for (size_t i=0; i<hand_size; i++)
    {
        SDL_Surface* button_surface = get_overlay(&data->hand_buttons, i);

        if (i<data->player_hand.num_tiles)
            blit_tile(data->tiles, data->player_hand.tiles[i],button_surface, tile_border, tile_border);
        else
            blit_tile(data->tiles, 0, button_surface, tile_border, tile_border);
    }

    blit_buttons(&data->hand_buttons, state->draw_surface);
}
#include "game.h"

#include <stdlib.h>

size_t find_line_end(const grid_t* grid, pos_t pos, bool max, bool horizontal)
{
    size_t out_val;
    pos_t search_pos = pos;
    if (!horizontal)
    {
        out_val = pos.y;
        search_pos.y = (max) ? search_pos.y+1 : search_pos.y-1;
    }
    else
    {
        out_val = pos.x;
        search_pos.x = (max) ? search_pos.x+1 : search_pos.x-1;
    }
    while (tile_valid(read_tile(grid, search_pos)))
    {
        if (!horizontal)
        {
            out_val = search_pos.y;
            search_pos.y = (max) ? search_pos.y+1 : search_pos.y-1;
        }
        else
        {
            out_val = search_pos.x;
            search_pos.x = (max) ? search_pos.x+1 : search_pos.x-1;
        }
    }
    return out_val;
}

bool verify_legal_line(const grid_t* grid, pos_t pos, tile_t tile, bool horizontal)
{
    size_t min_val = find_line_end(grid, pos, false, horizontal);
    size_t max_val = find_line_end(grid, pos, true, horizontal);

    uint8_t colour = tile_colour(tile);
    uint8_t count  = tile_count(tile);
    uint8_t shape  = tile_shape(tile);

    for (size_t search_val = min_val; search_val<max_val+1; search_val++)
    {
        pos_t search_pos = pos;
        if (horizontal)
            search_pos.x = search_val;
        else
            search_pos.y = search_val;

        if (search_pos.x == pos.x && search_pos.y == pos.y)
            continue;

        tile_t search_tile = read_tile(grid, search_pos);

        bool colour_common = colour == tile_colour(search_tile);
        bool count_common = count == tile_count(search_tile);
        bool shape_common = shape == tile_shape(search_tile);

        if (colour_common && count_common && shape_common)
            return false;
        else if (!colour_common && !count_common && !shape_common)
            return false;
    }

    return true;
}

bool verify_legal(const grid_t* grid, pos_t pos, tile_t tile)
{
    if (tile_valid(read_tile(grid, pos)))
        return false;

    if (!verify_legal_line(grid, pos, tile, true) || !verify_legal_line(grid, pos, tile, false))
        return false;

    for (int x_offset = -1; x_offset<2; x_offset++)
    {
        for (int y_offset = -1; y_offset<2; y_offset++)
        {
            if ((x_offset == 0 && y_offset != 0) || (x_offset != 0 && y_offset == 0))
            {
                if (tile_valid(grid->data[pos.x+x_offset][pos.y+y_offset]))
                    return true;
            }
        } 
    }

    return false;
}

game_state_t* init_game()
{
    game_state_t* game = malloc(sizeof(game_state_t));

    game->grid = init_grid();

    game->num_players = 0;
    game->num_tiles = bag_size;

    fill_bag(game->bag);
    return game;
}

void fill_bag(tile_t* bag)
{
    size_t bag_idx=0;
    for (size_t i=0; i<3; i++)
        for (size_t colour=0; colour<3; colour++)
            for (size_t count=0; count<3; count++)
                for (size_t shape=0; shape<3; shape++)
                {
                    bag[bag_idx] = 0b01000000 | colour | (count<<2) | (shape<<4);
                    ++bag_idx;
                }

    // Shuffle the bag
    // https://benpfaff.org/writings/clc/shuffle.html
    {
        for (size_t i=0; i<bag_size-1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (bag_size - i) + 1);
            tile_t temp = bag[j];
            bag[j] = bag[i];
            bag[i] = temp;
        }
    }
}

void push_tile_hand(hand_t* hand, tile_t tile)
{
    hand->tiles[hand->num_tiles] = tile;
    ++hand->num_tiles;
}

void pop_tile_hand(hand_t* hand, size_t idx)
{
    for (size_t i=idx; i<hand->num_tiles-1; i++)
    {
        hand->tiles[i] = hand->tiles[i+1];
    }
    --hand->num_tiles;
}

void play_tile(turn_t* turn, tile_t tile, pos_t pos)
{
    turn->poses[turn->num_tiles] = pos;
    turn->tiles[turn->num_tiles] = tile;
    ++turn->num_tiles;
}
#include "grid.h"

#include "game.h"
#include <stdio.h>

grid_t* init_grid()
{
    grid_t* grid = malloc(sizeof(grid_t));

    for (size_t i=0; i<grid_size; i++)
        for (size_t j=0; j<grid_size; j++)
            grid->data[i][j] = 0;

    return grid;
}

// Get value of tile at pos, returns invalid tile if it's never been read
tile_t read_tile(const grid_t* grid, pos_t pos)
{
    return grid->data[pos.x][pos.y];
}

bool tile_valid(tile_t tile)
{
    return tile & 0b01000000;
}

uint8_t tile_colour(tile_t tile)
{
    return tile&0x03;
}

uint8_t tile_count(tile_t tile)
{
    return (tile>>2)&0x03;
}

uint8_t tile_shape(tile_t tile)
{
    return (tile>>4)&0x03;
}

tile_t generate_tile(uint8_t colour, uint8_t count, uint8_t shape)
{
    tile_t tile = 0b01000000;
    tile |= colour;
    tile |= count<<2;
    tile |= shape<<4;
    return tile;
}

bool set_tile(grid_t* grid, pos_t pos, tile_t tile, bool first_tile)
{
    if (!verify_legal(grid, pos, tile) && !first_tile)
        return false;

    grid->data[pos.x][pos.y] = tile;

    return true;
}

bool find_corners(const grid_t* grid, size_t* min_x, size_t* max_x, size_t* min_y, size_t* max_y)
{
    bool found = false;
    *min_x = grid_size;
    *min_y = grid_size;
    *max_x = 0;
    *max_y = 0;
    for (size_t x=0; x<grid_size; x++)
    {
        for (size_t y=0; y<grid_size; y++)
        {
            pos_t pos = {x,y};
            tile_t tile = read_tile(grid, pos);

            if (tile_valid(tile))
            {
                found = true;
                if (x < *min_x)
                    *min_x = x;
                if (x > *max_x)
                    *max_x = x;

                if (y < *min_y)
                    *min_y = y;
                if (y > *max_y)
                    *max_y = y;
            }
        }
    }
    return found;
}
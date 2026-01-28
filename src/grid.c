#include "grid_impl.h"

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

bool set_tile(grid_t* grid, pos_t pos, tile_t tile, bool first_tile)
{
    if (!verify_legal(grid, pos, tile) && !first_tile)
        return false;

    grid->data[pos.x][pos.y] = tile;
    return true;
}
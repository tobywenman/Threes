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

// TODO test if legal!
bool set_tile(grid_t* grid, pos_t pos, tile_t tile)
{
    grid->data[pos.x][pos.y] = tile;
}
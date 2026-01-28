#include "grid.h"
#include "cli.h"

#include <stdio.h>

int main()
{
    grid_t* grid = init_grid();

    pos_t pos = {100,100};
    tile_t tile = 0b01000000;

    set_tile(grid, pos, tile);
    pos.x = 100;
    pos.y = 101;
    tile = 0b01000001;
    set_tile(grid, pos, tile);
    pos.x = 101;
    pos.y = 101;
    tile = 0b01100100;
    set_tile(grid, pos, tile);
    pos.x = 102;
    pos.y = 101;
    tile = 0b01010000;
    set_tile(grid, pos, tile);

    printf("test tile! %d\n", read_tile(grid, pos));

    print_grid(grid);

    return 0;
}
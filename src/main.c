#include "grid.h"

#include <stdio.h>

int main()
{
    grid_t* grid = init_grid();

    pos_t pos = {100,100};

    set_tile(grid, pos, 2);

    printf("test tile! %d\n", read_tile(grid, pos));

    return 0;
}
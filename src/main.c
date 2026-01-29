#include "cli.h"
#include "game.h"
#include <stdio.h>

int main()
{
    grid_t* grid = init_grid();

    pos_t pos = {100,100};
    tile_t tile = 0b01000000;

    set_tile(grid, pos, tile, true);
    print_grid(grid);
    printf("\n");
    pos.x = 100;
    pos.y = 101;
    tile = 0b01000001;
    set_tile(grid, pos, tile, false);
    print_grid(grid);
    printf("\n");
    pos.x = 101;
    pos.y = 101;
    tile = 0b01100100;
    set_tile(grid, pos, tile, false);
    print_grid(grid);
    printf("\n");
    pos.x = 102;
    pos.y = 101;
    tile = 0b01010000;
    set_tile(grid, pos, tile, false);
    print_grid(grid);
    printf("\n");

    game_state_t* game = init_game();

    for (size_t i=0; i<bag_size; i++)
        print_tile(tile_to_string(game->bag[i], pos));

    return 0;
}
#pragma once

#include "grid.h"
#include <stdlib.h>

// Maximum possible size for the grid due to the limited available tiles
// Previously I tried a chunk thing with a hash-map.
// Not worth it. This is only 64KiB of storage...

// constexpr size_t grid_size = 256;
#define grid_size 256 // GRR why doesn't vscode support c23 yet?!

typedef struct grid_t
{
    tile_t data[grid_size][grid_size];
}grid_t;


bool verify_legal(const grid_t* grid, pos_t pos, tile_t tile);
bool verify_legal_line(const grid_t* grid, pos_t pos, tile_t tile, int x_offset, int y_offset);
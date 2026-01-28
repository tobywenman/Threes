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
};
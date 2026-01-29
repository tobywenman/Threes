#pragma once

#include "grid.h"
#include <stdlib.h>

bool verify_legal(const grid_t* grid, pos_t pos, tile_t tile);
bool verify_legal_line(const grid_t* grid, pos_t pos, tile_t tile, bool horizontal);
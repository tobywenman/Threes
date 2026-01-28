#pragma once

#include "grid_impl.h"

#define symbol_col_count 6
#define symbol_row_count 3

typedef struct tile_string_t
{
    char data[symbol_row_count][symbol_col_count];
}tile_string_t;

void print_grid(const grid_t* grid);

tile_string_t tile_to_string(tile_t tile);

void print_tile(tile_string_t tile_string);
#pragma once

#include <stdint.h>
#include <stdlib.h>

// compressed description of a tile
// Wavedrom representation
// {reg: [
//     {bits: 2, name: 'colour', attr: ['YELLOW', 'ORANGE', 'PURPLE']  , type: 4},
//     {bits: 2, name: 'count',  attr: ['1', '2', '3']                 , type: 4},
//     {bits: 2, name: 'shape',  attr: ['CIRCLE', 'SQUARE', 'TRIANGLE'], type: 4},
//     {bits: 1, name: 'valid', type: 3},
//     {bits: 1, name: 'reserved', type: 2},
// ],}
typedef uint8_t tile_t;

// Maximum possible size for the grid due to the limited available tiles
// Previously I tried a chunk thing with a hash-map.
// Not worth it. This is only 64KiB of storage...

// constexpr size_t grid_size = 256;
#define grid_size 256 // GRR why doesn't vscode support c23 yet?!

typedef struct grid_t
{
    tile_t data[grid_size][grid_size];
}grid_t;


typedef struct pos_t
{
    size_t x,y;
}pos_t;

typedef struct grid_t grid_t;

bool tile_valid(tile_t tile);

uint8_t tile_colour(tile_t tile);

uint8_t tile_count(tile_t tile);

uint8_t tile_shape(tile_t tile);

tile_t generate_tile(uint8_t colour, uint8_t count, uint8_t shape);

grid_t* init_grid();

// Get value of tile at pos, returns invalid tile if it's never been read
tile_t read_tile(const grid_t* grid, pos_t pos);

// Set the value of a tile, will allocate a new one if it's never been written before
// Checks if it's a legal play, if not return false
bool set_tile(grid_t* grid, pos_t pos, tile_t tile, bool first_tile);

bool find_corners(const grid_t* grid, size_t* min_x, size_t* max_x, size_t* min_y, size_t* max_y);
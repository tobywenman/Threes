#include <stdint.h>

// compressed description of a tile
// Wavedrom representation
// {reg: [
//     {bits: 2, name: 'colour', attr: ['YELLOW', 'ORANGE', 'PURPLE']  , type: 4},
//     {bits: 2, name: 'count',  attr: ['1', '2', '3']                 , type: 4},
//     {bits: 2, name: 'shape',  attr: ['CIRCLE', 'SQUARE', 'TRIANGLE'], type: 4},
//     {bits: 1, name: 'valid', type: 3},
//   	{bits: 1, name: 'reserved', type: 2},
// ],}
typedef uint8_t tile_t;

typedef struct pos_t
{
    int x,y;
}pos_t;

typedef struct grid_t grid_t;

// Get value of tile at pos, returns invalid tile if it's never been read
tile_t read_tile(const grid_t* grid, pos_t pos);

// Set the value of a tile, will allocate a new one if it's never been written before
// Checks if it's a legal play, if not return false
bool set_tile(grid_t* grid, pos_t pos, tile_t tile);

// Return the most top left position that's been written to
pos_t get_min_pos(const grid_t* grid);
// Return the most bottom right position that's been written to
pos_t get_max_pos(const grid_t* grid);
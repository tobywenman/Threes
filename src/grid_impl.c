#include "grid_impl.h"

bool verify_legal_line(const grid_t* grid, pos_t pos, tile_t tile, int x_offset, int y_offset)
{
    bool colour_common = true;
    bool shape_common  = true;
    bool count_common  = true;

    uint8_t colour = tile_colour(tile);
    uint8_t count  = tile_count(tile);
    uint8_t shape  = tile_shape(tile);

    pos.x += x_offset;
    pos.y += y_offset;

    while (tile_valid(grid->data[pos.x][pos.y]))
    {
        if (colour != tile_colour(grid->data[pos.x][pos.y]))
            colour_common = false;
        if (count != tile_count(grid->data[pos.x][pos.y]))
            count_common = false;
        if (shape != tile_shape(grid->data[pos.x][pos.y]))
            shape_common = false;

        if (colour_common && count_common && shape_common)
            return false;
        else if (!colour_common && !count_common && !shape_common)
            return false;

        pos.x += x_offset;
        pos.y += y_offset;
    }
    return true;
}

bool verify_legal(const grid_t* grid, pos_t pos, tile_t tile)
{
    bool found_neighbour = false;

    for (int x_offset = -1; x_offset<2; x_offset++)
    {
        for (int y_offset = -1; y_offset<2; y_offset++)
        {
            if ((x_offset == 0 && y_offset != 0) || (x_offset != 0 && y_offset == 0))
            {
                if (!verify_legal_line(grid, pos, tile, x_offset, y_offset))
                    return false;
                if (tile_valid(grid->data[pos.x+x_offset][pos.y+y_offset]))
                    found_neighbour = true;
            }
        } 
    }

    return found_neighbour;
}
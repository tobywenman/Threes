#include "cli.h"

#include <stdio.h>
#include <string.h>

const tile_string_t blank = {{"      ",
                              "      ",
                              "      "}};


const tile_string_t square   = {{"+----+",
                                 "|    |",
                                 "+----+"}};

const tile_string_t triangle = {{"  /\\  ",
                                " /  \\ ",
                                "'----'"}};

const tile_string_t circle   = {{" .--. ",
                                 "(    )",
                                 " `--' "}};

tile_string_t tile_to_string(tile_t tile, pos_t pos)
{
    if (!(tile&0b01000000))
    {
        tile_string_t output = blank;

        char coords[10];
        sprintf(coords, "%02lx,%02lx,", pos.x, pos.y);
        memcpy(output.data[1], coords, symbol_col_count);
        return output;
    }

    tile_string_t output;
    uint8_t colour = tile&0x03;
    uint8_t count = (tile>>2)&0x03;
    uint8_t shape = (tile>>4)&0x03;

    switch (shape)
    {
    case 0: // CIRCLE
        output = circle;
        break;
    case 1: // SQUARE
        output = square;
        break;
    case 2: // TRIANGLE
        output = triangle;
        break;
    }

    switch (colour)
    {
    case 0: // Yellow
        output.data[1][2] = 'Y';
        break;
    case 1: // Orange
        output.data[1][2] = 'O';
        break;
    case 2: // Purple
        output.data[1][2] = 'P';
        break;
    }

    output.data[1][3] = '1'+count;

    return output;
}

void print_tile(tile_string_t tile_string)
{
    for (size_t i=0; i<symbol_row_count; i++)
    {
        for(size_t j=0; j<symbol_col_count; j++)
            putchar(tile_string.data[i][j]);
        putchar('\n');
    }
}

void print_grid(const grid_t* grid)
{
    size_t min_row, min_col, max_row, max_col;

    bool found = false;

    min_row = grid_size;
    min_col = grid_size;
    max_row = 0;
    max_col = 0;

    tile_string_t tile_strings[grid_size][grid_size];
    for (size_t row=0; row<grid_size; row++)
    {
        for (size_t col=0; col<grid_size; col++)
        {
            pos_t pos = {row, col};
            tile_strings[row][col] = tile_to_string(read_tile(grid, pos), pos);

            if (read_tile(grid, pos) & 0b01000000)
            {
                found = true;
                if (row < min_row)
                    min_row = row;
                if (col < min_col)
                    min_col = col;
                if (row > max_row)
                    max_row = row;
                if (col > max_col)
                    max_col = col;
            }
        }
    }

    constexpr size_t border_size = 1;
    
    min_row -= border_size;
    min_col -= border_size;
    max_row += border_size;
    max_col += border_size;

    if (found)
    {
        for (size_t row=min_row; row<max_row+1; row++)
        {
            for (size_t sub_row=0; sub_row<symbol_row_count; sub_row++)
            {
                for (size_t col=min_col; col<max_col+1; col++)
                    for (size_t char_idx=0; char_idx<symbol_col_count; char_idx++)
                        putchar(tile_strings[row][col].data[sub_row][char_idx]);
    
                putchar('\n');
            }
        }
    }

}
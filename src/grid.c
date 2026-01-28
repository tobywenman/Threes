#include "grid_impl.h"

grid_t init_grid()
{
    grid_t initial_grid;

    initial_grid.chunk_data = malloc(sizeof(chunk_t));
    initial_grid.num_chunks = 0;
    initial_grid.chunk_container_size = 1;

    initial_grid.pos_map = malloc(sizeof(pos_map_entry_t));
    initial_grid.pos_map_occupancy = 0;
    initial_grid.pos_map_size = 1;

    return initial_grid;
}

pos_t get_chunk_pos(pos_t global_pos)
{
    pos_t chunk_pos;
    chunk_pos.x = global_pos.x / chunk_size;
    chunk_pos.y = global_pos.y / chunk_size;

    return chunk_pos;
}
size_t get_chunk_x(pos_t global_pos)
{
    return global_pos.x % chunk_size;
}
size_t get_chunk_y(pos_t global_pos)
{
    return global_pos.y % chunk_size;
}

tile_t read_tile(const grid_t* grid, pos_t pos)
{
    size_t chunk_idx;
    if (do_map_search(&chunk_idx, grid->pos_map, grid->pos_map_size, get_chunk_pos(pos)))
    {
        return grid->chunk_data[chunk_idx].tiledata[get_chunk_x(pos)][get_chunk_y(pos)];
    }

    return 0;
}

// TODO check legality!
bool set_tile(grid_t* grid, pos_t pos, tile_t tile)
{
    size_t chunk_idx;
    if (!do_map_search(&chunk_idx, grid->pos_map, grid->pos_map_size, get_chunk_pos(pos)))
    {
        chunk_idx = grid->num_chunks;
        add_to_map(&grid->pos_map, &grid->pos_map_occupancy, &grid->pos_map_size, get_chunk_pos(pos));
        add_chunk(&grid->chunk_data, &grid->chunk_container_size, &grid->num_chunks);
    }

    grid->chunk_data[chunk_idx].tiledata[get_chunk_x(pos)][get_chunk_y(pos)] = tile;
}
#include "grid_impl.h"

#include <stdlib.h>


size_t pos_hash_func(pos_t pos, size_t pos_map_size)
{
    // Just a bunch of xors and shifts.
    // Hopefully this isn't terrble
    size_t input_xor = pos.x ^ pos.y;
    size_t output = 0;

    output ^= input_xor >> 16;
    output ^= input_xor >> 8;
    output ^= input_xor >> 4;
    output ^= input_xor >> 2;
    output ^= input_xor >> 1;

    return output % pos_map_size;
}

bool do_map_search(size_t* idx, const pos_map_entry_t* pos_map, size_t pos_map_size, pos_t pos)
{
    size_t search_idx = pos_hash_func(pos, pos_map_size);

    bool found = false;

    while (pos_map[search_idx].valid)
    {
        if (pos_map[search_idx].pos.x == pos.x && pos_map[search_idx].pos.y == pos.y)
        {
            found = true;
            *idx = search_idx;
            break;
        }
        ++search_idx;
        if (search_idx == pos_map_size)
            search_idx = 0;
    }
    return found;
}

void add_chunk(chunk_t** chunk_data, size_t* chunk_container_size, size_t* num_chunks)
{
    if (*num_chunks == *chunk_container_size)
    {
        *chunk_container_size *= 2;
        *chunk_data = realloc(*chunk_data, *chunk_container_size);
    }
    ++*num_chunks;
}
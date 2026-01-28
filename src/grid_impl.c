#include "grid_impl.h"

#define occupancy_factor 4

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

// Super Simple dynamic array, grow by POT each time it hits the top
void add_chunk(chunk_t** chunk_data, size_t* chunk_container_size, size_t* num_chunks)
{
    if (*num_chunks == *chunk_container_size)
    {
        *chunk_container_size *= 2;
        *chunk_data = realloc(*chunk_data, *chunk_container_size);
    }
    ++*num_chunks;
}

pos_map_entry_t* realloc_map(pos_map_entry_t* pos_map, size_t old_size, size_t new_size)
{
    pos_map_entry_t* new_map = malloc(sizeof(pos_map_entry_t)*new_size);

    for (size_t i=0; i<new_size; i++)
    {
        new_map[i].valid = false;
    }

    for (size_t i=0; i<old_size; i++)
    {
        size_t new_map_idx = pos_hash_func(pos_map[i].pos, new_size);

        // Find free slot
        while (new_map[new_map_idx].valid)
        {
            ++new_map_idx;
            if (new_map_idx == new_size)
                new_map_idx = 0;
        }
        new_map[new_map_idx].valid = true;
        new_map[new_map_idx].pos = pos_map[i].pos;
        new_map[new_map_idx].idx = pos_map[i].idx;
    }
    free(pos_map);
    return new_map;
}

void add_to_map(pos_map_entry_t** pos_map, size_t* pos_map_occupancy, size_t* pos_map_size, pos_t pos)
{
    if ((*pos_map_occupancy - *pos_map_size) > *pos_map_size/occupancy_factor);
    {
        *pos_map = realloc_map(*pos_map, *pos_map_size, *pos_map_size*2);
        *pos_map_size *= 2;
    }

    size_t search_idx = pos_hash_func(pos, *pos_map_size);
    
    while ((*pos_map)[search_idx].valid)
    {
        ++search_idx;
        if (search_idx == *pos_map_size)
        search_idx = 0;
    }
    (*pos_map)[search_idx].valid = true;
    (*pos_map)[search_idx].pos = pos;
    (*pos_map)[search_idx].idx = *pos_map_occupancy;
    ++*pos_map_occupancy;
}
#include "grid.h"
#include <stdlib.h>

// constexpr size_t chunk_size = 8;
#define chunk_size 8 // GRR why doesn't vscode support c23 yet?!

typedef struct chunk_t
{
    tile_t tiledata[chunk_size][chunk_size];
}chunk_t;

typedef struct pos_map_entry_t
{
    bool valid;
    pos_t pos;
    size_t idx;
}pos_map_entry_t;

typedef struct grid_t
{
    // Dynamic array for storing chunks
    // Indexed into using hash-map thing below
    chunk_t* chunk_data;
    size_t num_chunks;
    size_t chunk_container_size;

    pos_map_entry_t* pos_map;
    // How many entries are populated in hash map
    size_t pos_map_occupancy;
    // Total size of container as power of 2
    size_t pos_map_size;
}grid_t;

// Get starting index of search in hashmap
size_t pos_hash_func(pos_t pos, size_t pos_map_size);

bool do_map_search(size_t* idx, const pos_map_entry_t* pos_map, size_t pos_map_size, pos_t pos);

// Push new chunk into container
void add_chunk(chunk_t** chunk_data, size_t* chunk_container_size, size_t* num_chunks);
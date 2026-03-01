#pragma once

#define PORT "1612"

#include "game.h"

#include <pthread.h>

typedef struct player_network_t
{
    pthread_t listen_thread;
    int sock;
}player_network_t;

typedef struct thread_player_stack_t
{
    // No atomics or anything special is needed here as only one thread modifies the stack
    player_network_t data[max_players];
    size_t num_players;
}thread_player_stack_t;

void wait_for_client(thread_player_stack_t* players);

typedef struct listen_thread_data_t
{
    player_network_t* player_network;
    size_t id;
}listen_thread_data_t;

void* listen_for_packets(void* thread_data); // function called by each player thread
#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <pthread.h>

#include "network_packets.h"

#define PORT "1612"

typedef struct network_data_t
{
    int sock;

    pthread_t rec_thread;
}network_data_t;

bool connect_to_server(network_data_t* network_data, char* server_name);

void send_client_packet(network_data_t* network_data, client_packet_t packet);
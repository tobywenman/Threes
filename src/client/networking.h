#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "1612"

typedef struct network_data_t
{
    int sock;
}network_data_t;

bool connect_to_server(network_data_t* network_data, char* server_name);
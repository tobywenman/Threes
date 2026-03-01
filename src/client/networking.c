#include "networking.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool connect_to_server(network_data_t* network_data, char* server_name)
{
    bool connected = false;
    int status;
    
    struct addrinfo hints, *p;
    struct addrinfo* servinfo;

    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    if ((status = getaddrinfo(server_name, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        goto exit;
    }

    

    int sockfd;
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        inet_ntop(p->ai_family,
            get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
        printf("client: attempting connection to %s\n", s);

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }

        network_data->sock = sockfd;

        connected = true;

        break;
    }

    exit:
    return connected;
}

void send_client_packet(network_data_t* network_data, client_packet_t packet)
{
    char* packet_data;
    int len;
    switch (packet.type)
    {
        // Dataless packets
        case NOT_READY:
        case READY:
        case EXIT:
        // Something, something big endian...
            len = sizeof(client_packets_e);
            packet_data = malloc(len);
            memcpy(packet_data, &packet.type, len);
            break;
        case PLAY_TURN:
        case EXCHANGE:
    }

    
    while (len>0)
    {
        int bytes_sent;
        if ((bytes_sent = send(network_data->sock, packet_data, len, 0)) != -1)
            len -= bytes_sent;
        else
        {
            perror("client: can't send");
            exit(1);
        }
    }

    free(packet_data);
};
#include "server_networking.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define BACKLOG 10   // how many pending connections queue will hold

// get sockaddr, IPv4 or IPv6:
static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int wait_for_client()
{
    int sock;

    char s[INET6_ADDRSTRLEN];
    struct addrinfo hints, *servinfos;
    struct sockaddr_storage their_addr; // connector's address info

    socklen_t sin_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    int error_code;
    if ((error_code = getaddrinfo(NULL, PORT, &hints, &servinfos)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error_code));
        return 1;
    }
    struct addrinfo* servinfo;
    for (servinfo=servinfos; servinfo!=nullptr; servinfo=servinfo->ai_next)
    {
        if ((sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        int yes=1;

        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        inet_ntop(servinfo->ai_family,get_in_addr((struct sockaddr *)servinfo->ai_addr),s, sizeof s);
        printf("client: attempting connection to %s\n", s);

        if (bind(sock, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
            close(sock);
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfos);

    if (servinfo == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    int new_fd;
    

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sock, (struct sockaddr *)&their_addr,
            &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        close(new_fd);  // parent doesn't need this
    }
}
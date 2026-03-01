#pragma once

#include "game.h"

typedef enum client_packets_e : uint8_t
{
    READY,
    NOT_READY,
    PLAY_TURN,
    EXCHANGE,
    EXIT,
}client_packets_e;

typedef enum server_packets_e : uint8_t
{
    ADD_PLAYER,
    PLAYER_TURN,
    ERROR,
    END,
}server_packets_e;

typedef union client_packet_u
{
    turn_t turn;
    // Not all packets have data
}client_packet_u;

typedef struct client_packet_t
{
    client_packets_e type;
    client_packet_u packet;
}client_packet_t;

#pragma once

#include "game.h"

typedef enum client_packets_e
{
    PLAY_TURN,
    EXCHANGE,
    EXIT,
}send_type_e;

typedef enum server_packets_e
{
    ADD_PLAYER,
    PLAYER_TURN,
    ERROR,
    END,
}send_type_e;

typedef union client_packet_u
{
    turn_t turn;
    // No data for exchange or exit
};

typedef struct client_packet_t
{
    send_type_e type;
    client_packet_u packet;
}client_packet_t;

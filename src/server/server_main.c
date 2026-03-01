#include "cli.h"
#include "game.h"
#include <stdio.h>

#include "server_networking.h"

int main()
{
    thread_player_stack_t player_stack;
    player_stack.num_players = 0;
    wait_for_client(&player_stack);

    return 0;
}
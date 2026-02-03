#pragma once

typedef enum state_e
{
    SPLASH,
    MENU,
    SERVER_JOIN,
    GAME,
    QUIT,
}state_e;

typedef struct main_state_t main_state_t;

bool call_state_main(main_state_t* state);

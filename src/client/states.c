#include "states.h"
#include "client_main.h"
#include "splash.h"
#include "menu.h"
#include "server_join.h"

bool call_state_main(main_state_t* state)
{
    switch (state->state)
    {
    case SPLASH:
        return splash_main(state);
    case MENU:
        return menu_main(state);
    case SERVER_JOIN:
        return server_join_main(state);
    // case GAME:
    //     return game_main(state);
    // case QUIT:
    //     return quit_main(state);
    }
}

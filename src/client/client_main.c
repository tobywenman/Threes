#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>

#include "client_state.h"

int main()
{
    client_state_t* state = init_state();

    if (state != NULL)
        while (call_scene_func(state)) {}

    free(state);

    return 0;
}
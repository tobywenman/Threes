#include "splash.h"

#include <stdlib.h>

void splash_init(main_state_t* state)
{
    TTF_Init();

    state->state_data = malloc(sizeof(splash_state_t));

    ((splash_state_t*)(state->state_data))->font = TTF_OpenFont(font_path, 16);
}

bool splash_main(main_state_t* state)
{
    splash_state_t* state_data = state->state_data;

    SDL_Event e;
    SDL_zero(e);
    //Get event data
    while(SDL_PollEvent(&e) == true)
    {
        //If event is quit type
        if(e.type == SDL_EVENT_QUIT)
        {
            //End the main loop
            return false;
        }
    }
    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
    SDL_Color colour = {0,0,0,0xff};
    SDL_BlitSurface(TTF_RenderText_Blended(state_data->font, "Threees!", 0, colour), NULL, state->draw_surface, NULL);
}
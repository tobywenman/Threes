#include "splash.h"
#include "menu.h"

#include <stdlib.h>

void splash_init(main_state_t* state)
{
    TTF_Init();

    TTF_Font* font = TTF_OpenFont(font_path, 16);

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
    SDL_Color colour = {0,0,0,0xff};
    SDL_BlitSurface(TTF_RenderText_Blended(font, "Threees!", 0, colour), NULL, state->draw_surface, NULL);
    SDL_Rect subtext_rect = {0, px_height/2, px_width, px_height};
    SDL_BlitSurface(TTF_RenderText_Blended(font, "Press any key to continue", 0, colour), NULL, state->draw_surface, &subtext_rect);
}

bool splash_main(main_state_t* state)
{

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
        if(e.type == SDL_EVENT_KEY_DOWN)
        {
            menu_init(state);
        }
    }
    return true;
}
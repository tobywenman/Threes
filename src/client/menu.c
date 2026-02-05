#include "menu.h"

void menu_init(main_state_t* state)
{
    menu_data_t* data = malloc(sizeof(menu_data_t));

    state->state_data = data;

    state->state = MENU;

    data->button_manager = init_button_manager();

    SDL_Rect rect = {50,50,100,100};
    add_button(&data->button_manager, rect, "button!");

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);

    blit_buttons(&data->button_manager, state->draw_surface);
}

bool menu_main(main_state_t* state)
{
    menu_data_t* data = state->state_data;

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
        if(e.type == SDL_EVENT_MOUSE_MOTION)
        {
            float width_mult = (float)state->draw_surface->w / (float)state->win_surface->w;
            float height_mult = (float)state->draw_surface->h / (float)state->win_surface->h;
            update_hover_mouse(&data->button_manager, e.motion.x*width_mult, e.motion.y*height_mult);
            blit_buttons(&data->button_manager, state->draw_surface);
        }
    }
    return true;
}
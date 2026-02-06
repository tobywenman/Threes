#include "menu.h"

void menu_init(main_state_t* state)
{
    menu_data_t* data = malloc(sizeof(menu_data_t));

    state->state_data = data;

    state->state = MENU;

    data->button_manager = init_button_manager();

    #define button_width px_width/2
    #define button_x px_width/4
    #define button_height 32
    #define button_space 16
    #define button_start_y 16

    SDL_Rect rect = {button_x,button_start_y,button_width,button_height};
    data->join_id = add_button(&data->button_manager, rect, "Connect to Server!");
    rect.y += button_height + button_space;
    data->settings_id = add_button(&data->button_manager, rect, "Settings.");
    rect.y += button_height + button_space;
    data->quit_id = add_button(&data->button_manager, rect, "Quit.");

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
            update_hover_mouse(&data->button_manager, e.motion.x*state->width_mult, e.motion.y*state->height_mult);
            blit_buttons(&data->button_manager, state->draw_surface);
        }
        if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            size_t butt_id;
            if(get_id_at_pos(&data->button_manager, &butt_id, e.button.x*state->width_mult, e.button.y*state->height_mult))
            {
                if (butt_id == data->join_id)
                {
                    destroy_menu(state);
                    return false;
                }
                else if (butt_id == data->settings_id)
                {
                    destroy_menu(state);
                    return false;
                }
                else if (butt_id == data->quit_id)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void destroy_menu(main_state_t* state)
{
    menu_data_t* data = state->state_data;

    destroy_buttons(&data->button_manager);

    free(data);
}
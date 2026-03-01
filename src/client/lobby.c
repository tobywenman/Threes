#include "lobby.h"

#include <stdio.h>

#include "font_paths.h"

void lobby_init(main_state_t* state, network_data_t* network_data)
{
    lobby_data_t* data = malloc(sizeof(lobby_data_t));

    data->network_data = network_data;

    state->state_data = data;

    state->state = LOBBY;

    data->button_manager = init_button_manager();

    #define button_width px_width/2
    #define button_x px_width/4
    #define button_height 32
    #define button_space 16
    #define button_start_y 16

    SDL_Rect rect = {button_x,button_start_y,button_width,2*button_height};
    data->ready_id = add_button_text(&data->button_manager, rect, "Ready?");
    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
    blit_buttons(&data->button_manager, state->draw_surface);

    data->num_players = 1;

    data->numbers = load_numbers();

    TTF_Font* font = TTF_OpenFont(font_path, 16);

    print_numbers(state->draw_surface, data->numbers, button_x, button_start_y+64, data->num_players, 2);
}

bool lobby_main(main_state_t* state)
{
    lobby_data_t* data = state->state_data;

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
        if (e.type == SDL_EVENT_MOUSE_MOTION)
        {
            update_hover_mouse(&data->button_manager, e.motion.x*state->width_mult, e.motion.y*state->height_mult);
        }
        if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (e.button.button == 1)
            {
                button_id_t id;
                if (get_id_at_pos(&data->button_manager, &id, e.button.x*state->width_mult, e.button.y*state->height_mult))
                {
                    printf("toggling!\n");
                    toggle_selected(&data->button_manager, id);

                    client_packet_t packet;
                    if (get_selected(&data->button_manager, &id))
                    {
                        packet.type = READY;
                    }
                    else
                    {
                        packet.type = NOT_READY;
                    }

                    send_client_packet(data->network_data, packet);
                }
            }
        }

        blit_buttons(&data->button_manager, state->draw_surface);
    }
    return true;
}
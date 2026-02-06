#include "server_join.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "font_paths.h"

#define max_input_chars 1000

void server_join_init(main_state_t* state)
{
    server_join_data_t* data = malloc(sizeof(server_join_data_t));

    data->string_length = 0;
    data->input_string = malloc(max_input_chars);
    data->input_string[0] = '\0';
    data->font = TTF_OpenFont(font_path, 16);

    state->state_data = data;

    state->state = SERVER_JOIN;

    SDL_StartTextInput(state->win);

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
}

bool server_join_main(main_state_t* state)
{
    server_join_data_t* data = state->state_data;

    SDL_Event e;
    SDL_zero(e);

    bool need_text_update = false;

    //Get event data
    while(SDL_PollEvent(&e) == true)
    {
        //If event is quit type
        if(e.type == SDL_EVENT_QUIT)
        {
            //End the main loop
            return false;
        }
        if(e.type == SDL_EVENT_KEY_DOWN )
        {
            //Handle backspace
            if(e.key.key == SDLK_BACKSPACE && data->string_length > 0 )
            {
                //lop off character
                --data->string_length;
                data->input_string[data->string_length] = '\0';
                need_text_update = true;
            }
            //Handle copy
            if(e.key.key == SDLK_C && SDL_GetModState() & SDL_KMOD_CTRL)
            {
                SDL_SetClipboardText(data->input_string);
            }
            //Handle paste
            if(e.key.key == SDLK_V && SDL_GetModState() & SDL_KMOD_CTRL)
            {
                //Copy text from temporary buffer
                char* tempText = SDL_GetClipboardText();

                size_t clipboard_len = strlen(tempText)+1; // +1 includes null byte
                
                if (clipboard_len + data->string_length >= max_input_chars)
                {
                    clipboard_len = max_input_chars-data->string_length-1;
                    tempText[clipboard_len] = '\0';
                }


                memcpy(&data->input_string[data->string_length], tempText, clipboard_len);

                data->string_length += clipboard_len;

                SDL_free(tempText);

                need_text_update = true;
            }
        }
        if( e.type == SDL_EVENT_TEXT_INPUT )
        {
            printf("in text input state!\n");
            if(!(SDL_GetModState() & SDL_KMOD_CTRL))
            {
                printf("getting char %c!\n", e.text.text[0]);
                size_t text_idx = 0;
                while (data->string_length < max_input_chars-1 && e.text.text[text_idx] != '\0')
                {
                    printf("adding char\n");
                    data->input_string[data->string_length] = e.text.text[text_idx];
                    ++data->string_length;
                    ++text_idx;
                }

                data->input_string[data->string_length] = '\0';
                need_text_update = true;
            }
        }
    }
    if (need_text_update)
    {
        printf("updating text! %s\n", data->input_string);
        printf("string length: %lu\n", data->string_length);
        SDL_Color text_colour = {0,0,0, 0xff};
        SDL_Surface* text = TTF_RenderText_Blended_Wrapped(data->font, data->input_string, data->string_length-1, text_colour, 100);

        SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);
        SDL_BlitSurface(text, NULL, state->draw_surface, NULL);
        SDL_DestroySurface(text);
    }
    return true;
}
#include "server_join.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "font_paths.h"
#include "game_main.h"

static void draw_text_box(TTF_Font* font, const char* text, SDL_Rect rect, SDL_Surface* surface)
{
    SDL_Color text_colour = {0,0,0, 0xff};
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font, text, 0, text_colour, rect.w);

    SDL_Rect src_rect = rect;
    src_rect.x = 0;
    src_rect.y = 0;

    SDL_Rect border_rect = rect;
    border_rect.x -= 1;
    border_rect.y -= 1;
    border_rect.h += 2;
    border_rect.w += 2;

    SDL_FillSurfaceRect(surface, &border_rect, 0);
    SDL_FillSurfaceRect(surface, &rect, 0xFFFFFF);
    SDL_BlitSurface(text_surface, &src_rect, surface, &rect);
    SDL_DestroySurface(text_surface);
}

void server_join_init(main_state_t* state)
{
    server_join_data_t* data = malloc(sizeof(server_join_data_t));

    data->string_length = 0;
    data->input_string[0] = '\0';
    data->font = TTF_OpenFont(font_path, 16);

    state->state_data = data;

    state->state = SERVER_JOIN;

    SDL_StartTextInput(state->win);

    SDL_FillSurfaceRect(state->draw_surface, NULL, 0xFFFFFF);

    SDL_Rect text_pos = {40, 80, 240, 40};
    draw_text_box(data->font, data->input_string, text_pos, state->draw_surface);

    text_pos.y = 0;
    SDL_Color text_colour = {0,0,0, 0xff};
    SDL_Surface* text_surface = TTF_RenderText_Solid(data->font, "Enter server address", 0, text_colour);
    SDL_BlitSurface(text_surface, NULL, state->draw_surface, &text_pos);

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
                    clipboard_len = max_input_chars-data->string_length;
                    tempText[clipboard_len] = '\0';
                }


                memcpy(&data->input_string[data->string_length], tempText, clipboard_len);

                data->string_length += clipboard_len-1;

                SDL_free(tempText);

                need_text_update = true;
            }
            if (e.key.key == SDLK_RETURN)
            {
                size_t addr_len = strlen(data->input_string);

                char* server_addr = malloc(addr_len);

                strcpy(server_addr, data->input_string);

                server_join_destroy(state);

                game_main_init(state, server_addr);

                return true;
            }
        }
        if( e.type == SDL_EVENT_TEXT_INPUT )
        {
            if(!(SDL_GetModState() & SDL_KMOD_CTRL))
            {
                size_t text_idx = 0;
                while (data->string_length < max_input_chars-1 && e.text.text[text_idx] != '\0')
                {
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
        SDL_Rect text_pos = {40, 80, 240, 40};
        draw_text_box(data->font, data->input_string, text_pos, state->draw_surface);
    }
    return true;
}

void server_join_destroy(main_state_t* state)
{
    server_join_data_t* data = state->state_data;

    TTF_CloseFont(data->font);
    free(data);
}
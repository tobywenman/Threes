#include "button.h"

#include "font_paths.h"

button_manager_t init_button_manager()
{
    button_manager_t manager;

    manager.buttons = nullptr;
    manager.num_buttons = 0;
    manager.container_size = 0;

    manager.font = TTF_OpenFont(font_path, 16);

    return manager;
}

button_id_t add_button_text(button_manager_t* manager, SDL_Rect size_pos, char* text)
{
    // Grow container if needs be
    if (manager->num_buttons == manager->container_size)
    {
        manager->container_size = (manager->container_size == 0) ? 1 : manager->container_size*2;
        manager->buttons = realloc(manager->buttons, sizeof(button_t)*manager->container_size);
    }

    SDL_Color text_colour = {0,0,0, 0xff};

    manager->buttons[manager->num_buttons].size_pos = size_pos;
    manager->buttons[manager->num_buttons].norm_colour = 0x00ff00;
    manager->buttons[manager->num_buttons].hover_colour = 0x0000ff;
    manager->buttons[manager->num_buttons].hovering = false;
    manager->buttons[manager->num_buttons].selected = false;

    manager->buttons[manager->num_buttons].surface_overlay = TTF_RenderText_Blended_Wrapped(manager->font, text, 0, text_colour, size_pos.w);

    button_id_t id = manager->num_buttons;

    ++manager->num_buttons;

    return id;
}

button_id_t add_button_surface(button_manager_t* manager, SDL_Rect size_pos, SDL_Surface* surface)
{
    // Grow container if needs be
    if (manager->num_buttons == manager->container_size)
    {
        manager->container_size = (manager->container_size == 0) ? 1 : manager->container_size*2;
        manager->buttons = realloc(manager->buttons, sizeof(button_t)*manager->container_size);
    }

    manager->buttons[manager->num_buttons].size_pos = size_pos;
    manager->buttons[manager->num_buttons].norm_colour = 0x00ff00;
    manager->buttons[manager->num_buttons].hover_colour = 0x0000ff;
    manager->buttons[manager->num_buttons].hovering = false;
    manager->buttons[manager->num_buttons].selected = false;

    manager->buttons[manager->num_buttons].surface_overlay = surface;

    button_id_t id = manager->num_buttons;

    ++manager->num_buttons;

    return id;
}

bool get_id_at_pos(const button_manager_t* manager, button_id_t* id, float x, float y)
{
    for (size_t i=0; i<manager->num_buttons; i++)
    {
        float min_x = (float)manager->buttons[i].size_pos.x;
        float max_x = (float)(manager->buttons[i].size_pos.x + manager->buttons[i].size_pos.w);
        float min_y = (float)manager->buttons[i].size_pos.y;
        float max_y = (float)(manager->buttons[i].size_pos.y + manager->buttons[i].size_pos.h);

        if (x > min_x && x < max_x && y > min_y && y<max_y)
        {
            *id = i;
            return true;
        }
    }
    return false;
}

void update_hover_mouse(button_manager_t* manager, float x, float y)
{
    for (size_t i=0; i<manager->num_buttons; i++)
        manager->buttons[i].hovering = false;
    
    button_id_t id;
    if (get_id_at_pos(manager, &id, x, y))
        manager->buttons[id].hovering = true;
}

void select_button(button_manager_t* manager, button_id_t id)
{
    for (size_t i=0; i<manager->num_buttons; i++)
        manager->buttons[i].selected = false;

    manager->buttons[id].selected = true;
}

void blit_buttons(const button_manager_t* manager, SDL_Surface* target)
{
    for (size_t i=0; i<manager->num_buttons; i++)
    {
        button_t* but = &manager->buttons[i];
        if (but->hovering || but->selected)
            SDL_FillSurfaceRect(target, &but->size_pos, but->hover_colour);
        else
            SDL_FillSurfaceRect(target, &but->size_pos, but->norm_colour);

        SDL_BlitSurface(but->surface_overlay, NULL, target, &but->size_pos);
    }
}

void destroy_buttons(button_manager_t* manager)
{
    for (size_t i=0; i<manager->num_buttons; i++)
        SDL_DestroySurface(manager->buttons[i].surface_overlay);

    TTF_CloseFont(manager->font);

    free(manager->buttons);
    manager->container_size = 0;
    manager->num_buttons = 0;
}

SDL_Surface* get_overlay(button_manager_t* manager, size_t id)
{
    return manager->buttons[id].surface_overlay;
}
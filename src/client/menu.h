#pragma once

#include "client_main.h"

#include "button.h"

typedef struct menu_data_t
{
    button_manager_t button_manager;
}menu_data_t;

void menu_init(main_state_t* state);
bool menu_main(main_state_t* state);
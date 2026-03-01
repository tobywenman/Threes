#pragma once

#include <SDL3/SDL.h>

#define number_path "../src/client/img/numbers.bmp"

SDL_Surface* load_numbers();

void print_numbers(SDL_Surface* surface, SDL_Surface* numbers, size_t x, size_t y, uint64_t number, size_t num_digits);
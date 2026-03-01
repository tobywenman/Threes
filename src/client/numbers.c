#include "numbers.h"

#include <SDL3_image/SDL_image.h>

SDL_Surface* load_numbers()
{
    return IMG_Load(number_path);
}

void print_numbers(SDL_Surface* surface, SDL_Surface* numbers, size_t x, size_t y, uint64_t number, size_t num_digits)
{
    size_t number_size = numbers->h;

    SDL_Rect dst_rect;
    dst_rect.x = x+(num_digits-1)*number_size;
    dst_rect.y = y;
    dst_rect.h = number_size;
    dst_rect.w = number_size;

    uint64_t factor = 10;
    for (size_t i=0; i<num_digits; i++)
    {
        uint64_t digit = number%factor;

        number -= digit;

        SDL_Rect src_rect;
        src_rect.x = digit*number_size;
        src_rect.y = 0;
        src_rect.h = number_size;
        src_rect.w = number_size;
        SDL_BlitSurface(numbers, &src_rect, surface, &dst_rect);

        dst_rect.x -= number_size;
        factor *= 10;
    }
}
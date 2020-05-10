#include "my_sdl_util.h"

SDL_Surface *optimise_surface(SDL_Surface *surface, const SDL_Surface *opt_for)
{
    SDL_Surface *tmp;

    tmp = SDL_ConvertSurface(surface, opt_for->format, 0);

    if(tmp != NULL){
        SDL_FreeSurface(surface);
        surface = tmp;

        return surface;
    }

    return NULL;
}

SDL_Texture *accelerate_surface(SDL_Surface *surface, SDL_Renderer *renderer)
{
    SDL_Texture *result = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return result;
}

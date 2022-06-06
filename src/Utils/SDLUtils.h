#pragma once

#include <SDL2/SDL.h>

struct SDL_Deleter
{
    void operator()(SDL_Window* window) { SDL_DestroyWindow(window); };
    void operator()(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); };
    void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); };
};
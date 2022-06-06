#pragma once

#include <SDL2/SDL.h>
#include "../Event.h"

struct KeyPressedEvent : public Event
{
    SDL_Keysym m_keysym;
    KeyPressedEvent(SDL_Keysym keysym)
        : m_keysym(keysym)
    {
    }
};
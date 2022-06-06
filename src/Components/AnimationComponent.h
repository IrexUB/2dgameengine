#pragma once

#include <iostream>
#include <SDL2/SDL.h>

struct AnimationComponent
{
    uint32_t m_nbFrames;
    uint32_t m_framerate;
    uint32_t m_currentFrame;
    uint32_t m_startTime;

    AnimationComponent(uint32_t nbFrames = 0, uint32_t framerate = 0)
        : m_nbFrames(nbFrames),
          m_framerate(framerate),
          m_currentFrame(0),
          m_startTime(SDL_GetTicks())
    {
    }
};
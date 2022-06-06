#pragma once

#include <SDL2/SDL_image.h>
#include <string>

struct SpriteComponent
{
public:
    SDL_Rect m_srcRect;
    std::string m_spriteId;
    uint32_t m_zIndex;
    bool m_isFixed;

    SpriteComponent(std::string spriteId = "default-sprite", SDL_Rect srcRect = {0, 0, 0, 0}, uint32_t zIndex = 0, bool isFixed = false)
        : m_spriteId(std::move(spriteId)),
          m_srcRect(std::move(srcRect)),
          m_zIndex(zIndex),
          m_isFixed(isFixed)
    {
    }
};
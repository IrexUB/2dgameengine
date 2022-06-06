#pragma once

#include <glm/glm.hpp>

struct BoxColliderComponent
{
    uint32_t m_width, m_height;
    glm::vec2 m_offset;
    bool m_isVisible;
    
    BoxColliderComponent(uint32_t width = 0, uint32_t height = 0, glm::vec2 offset = {0, 0})
        : m_width(width),
          m_height(height),
          m_offset(std::move(offset))
    {
    }
};
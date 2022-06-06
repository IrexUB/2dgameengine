#pragma once

#include <glm/glm.hpp>

struct TransformComponent
{
    glm::vec2 m_position;
    glm::vec2 m_scale;
    float m_rotation;

    TransformComponent(glm::vec2 position = glm::vec2(0.0, 0.0), glm::vec2 scale = glm::vec2(1.0, 1.0), float rotation = 0.0)
        : m_position( std::move(position) ), m_scale( std::move(scale) ), m_rotation(rotation)
    {}
};
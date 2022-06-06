#pragma once

#include <glm/glm.hpp>

struct RigidBody2D
{
    glm::vec2 m_velocity;

    RigidBody2D(glm::vec2 velocity = glm::vec2(0.0, 0.0))
        : m_velocity(std::move(velocity))
    {
    }
};
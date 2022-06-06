#pragma once

#include <glm/glm.hpp>

struct KeyboardMovementComponent
{
    double m_speed;
    KeyboardMovementComponent(double speed = 1.0)
        : m_speed(speed)
    {}
};
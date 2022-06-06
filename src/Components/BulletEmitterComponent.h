#pragma once

#include <glm/glm.hpp>

struct BulletEmitterComponent
{
    float m_cooldown;
    float m_lastEmissionTime;
    glm::vec2 m_projectileVelocity;

    BulletEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0, 0), float cooldown = 0.0f)
        :   m_projectileVelocity(std::move(projectileVelocity)), 
            m_cooldown(cooldown * 1000)
    {
    }
};
#pragma once

#include "../Event.h"
#include "../../ECS/ECS.h"

struct CollisionEvent : public Event
{
    Entity m_a;
    Entity m_b;

    CollisionEvent(Entity a, Entity b)
        : m_a(a), m_b(b)
    {}
};
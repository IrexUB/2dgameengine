#pragma once

#include "../ECS/ECS.h"
#include "../Components/RigidBody2D.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/EventBus.h"
#include "../Events/EventsType/CollisionEvent.h"

class MovementSystem : public System
{
public:
    MovementSystem()
    {
        RequireComponent<RigidBody2D>();
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    };
 
    void SubscribeToEvents(EventBus* bus)
    {
    }

    void Update(double deltaTime)
    {
        for (auto &entity : GetSystemEntities())
        {
            auto &transform = entity.GetComponent<TransformComponent>();
            auto &rb2d = entity.GetComponent<RigidBody2D>();

            transform.m_position.x += rb2d.m_velocity.x * deltaTime;
            transform.m_position.y += rb2d.m_velocity.y * deltaTime;
        }
    }
};
#pragma once

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Events/EventBus.h"
#include "../Events/EventsType/CollisionEvent.h"

class Physics2dSystem : public System
{
public:
    Physics2dSystem()
    {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    inline bool AABBCollision(Entity a, Entity b) 
    {
        auto aCollider = a.GetComponent<BoxColliderComponent>();
        auto bCollider = b.GetComponent<BoxColliderComponent>();

        auto aTransform = a.GetComponent<TransformComponent>();
        auto bTransform = b.GetComponent<TransformComponent>();
        

        return aTransform.m_position.x < bTransform.m_position.x + bCollider.m_width &&
                aTransform.m_position.x + aCollider.m_width > bTransform.m_position.x &&
                aTransform.m_position.y < bTransform.m_position.y + bCollider.m_height &&
                aTransform.m_position.y + aCollider.m_height > bTransform.m_position.y;
    }

    void Update(EventBus* eventBus)
    {
        auto &entities = GetSystemEntities();

        for (auto& aEntity : entities) {
            for (auto& bEntity : entities) {
                if (aEntity == bEntity) continue;

                if (AABBCollision(aEntity, bEntity)) {
                    eventBus->EmitEvent<CollisionEvent>(aEntity, bEntity);
                }
            }
        }
    }
};
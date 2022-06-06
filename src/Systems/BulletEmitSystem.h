#pragma once

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBody2D.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BulletEmitterComponent.h"

class BulletEmitSystem : public System
{
public:
    BulletEmitSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BulletEmitterComponent>();
    }

    void Update(std::unique_ptr<Registry>& registry)
    {
        for (auto& entity : GetSystemEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& projectileEmitter = entity.GetComponent<BulletEmitterComponent>();

            if ((SDL_GetTicks() - projectileEmitter.m_lastEmissionTime) >= projectileEmitter.m_cooldown) {
                glm::vec2 newProjectilePosition = transform.m_position;

                if (entity.HasComponent<SpriteComponent>()) {
                    auto& spriteComponent = entity.GetComponent<SpriteComponent>();
                    newProjectilePosition.x += transform.m_scale.x * spriteComponent.m_srcRect.w;
                    newProjectilePosition.y += transform.m_scale.y * (spriteComponent.m_srcRect.h / 2);
                }
                

                auto newProjectile = registry->CreateEntity();
                newProjectile.AddComponent<TransformComponent>(newProjectilePosition, glm::vec2(1.0, 1.0), 0.0);
                newProjectile.AddComponent<SpriteComponent>("bullet", SDL_Rect(4, 4), 4);
                newProjectile.AddComponent<RigidBody2D>(projectileEmitter.m_projectileVelocity);
                newProjectile.AddComponent<BoxColliderComponent>(4, 4);


                projectileEmitter.m_lastEmissionTime = SDL_GetTicks();
            }
        }
    }
};
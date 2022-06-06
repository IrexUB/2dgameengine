#pragma once

#include "../ECS/ECS.h"
#include "../Events/EventBus.h"
#include "../Components/RigidBody2D.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardMovementComponent.h"
#include "../Events/EventsType/KeyPressedEvent.h"

class KeyboardMovementSystem : public System
{
public:
    KeyboardMovementSystem()
    {
        RequireComponent<RigidBody2D>();
        RequireComponent<SpriteComponent>();
        RequireComponent<KeyboardMovementComponent>();
    }

    void SubscribeToEvents(EventBus* eventBus)
    {
        eventBus->SubscribeToEvent<KeyPressedEvent>([&](KeyPressedEvent& event) {
            OnKeyPressed(event);
        });
    }

    void OnKeyPressed(KeyPressedEvent& keyPressedEvent) {
        for (auto& entity : GetSystemEntities()) {
            glm::vec2 direction = {0, 0};
            auto& spriteComponent = entity.GetComponent<SpriteComponent>();
            switch (keyPressedEvent.m_keysym.sym) {
                case SDLK_UP:
                    direction.y = -1;
                    spriteComponent.m_srcRect.y = spriteComponent.m_srcRect.h * 0;
                    break;
                case SDLK_RIGHT:
                    direction.x = 1;
                    spriteComponent.m_srcRect.y = spriteComponent.m_srcRect.h * 1;
                    break;
                case SDLK_DOWN:
                    direction.y = 1;
                    spriteComponent.m_srcRect.y = spriteComponent.m_srcRect.h * 2;
                    break;
                case SDLK_LEFT:
                    direction.x = -1;
                    spriteComponent.m_srcRect.y = spriteComponent.m_srcRect.h * 3;
                    break;
            } 

            auto& rb2d = entity.GetComponent<RigidBody2D>();

            auto& movementComponent = entity.GetComponent<KeyboardMovementComponent>();
            direction.x *= movementComponent.m_speed;
            direction.y *= movementComponent.m_speed;

            rb2d.m_velocity = direction;
            // Logger::LogDebug("Velocity: [" + std::to_string(rb2d.m_velocity.x) + ", " + std::to_string(rb2d.m_velocity.y) + "]");
        }
    }
};
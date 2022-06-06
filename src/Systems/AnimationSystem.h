#pragma once

#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"

class AnimationSystem : public System
{
public:
    AnimationSystem()
    {
        RequireComponent<AnimationComponent>();
        RequireComponent<SpriteComponent>();
    }

    void Update()
    {
        for (auto &entity : GetSystemEntities())
        {
            auto &sprite = entity.GetComponent<SpriteComponent>();
            auto &animator = entity.GetComponent<AnimationComponent>();

            animator.m_currentFrame = (((SDL_GetTicks() - animator.m_startTime) * animator.m_framerate) / 1000) % animator.m_nbFrames;
            sprite.m_srcRect.x = animator.m_currentFrame * sprite.m_srcRect.w;
        }
    }
};
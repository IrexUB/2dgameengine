#pragma once

#include <SDL2/SDL.h>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"

class CameraFollowSystem : public System
{
public:
    CameraFollowSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<CameraFollowComponent>();
    }

    void Update(SDL_Rect& camera)
    {
        for (auto& entity : GetSystemEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();

            camera.x = transform.m_position.x - (camera.w / 2);
            camera.y = transform.m_position.y - (camera.h / 2);
        }
    }
};
#pragma once

#include <SDL2/SDL.h>
#include <algorithm>

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../AssetsStore/AssetsStore.h"

class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<SpriteComponent>();
        RequireComponent<TransformComponent>();
    }

    inline void SortByZIndex()
    {
        auto &entities = GetSystemEntities();
        std::sort(entities.begin(), entities.end(), [&](const Entity &lhs, const Entity &rhs)
                  {
            auto &lhsSprite = lhs.GetComponent<SpriteComponent>();
            auto &rhsSprite = rhs.GetComponent<SpriteComponent>();

            return lhsSprite.m_zIndex < rhsSprite.m_zIndex; });
    }

    void Update(SDL_Renderer *renderer, AssetsStore *store, SDL_Rect& camera)
    {
        for (auto &entity : GetSystemEntities())
        {
            SortByZIndex();

            auto &transform = entity.GetComponent<TransformComponent>();
            auto &sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect dstRect = {
                static_cast<int>(transform.m_position.x) - ((sprite.m_isFixed) ? 0 : camera.x),
                static_cast<int>(transform.m_position.y) - ((sprite.m_isFixed) ? 0 : camera.y),
                sprite.m_srcRect.w * transform.m_scale.x,
                sprite.m_srcRect.h * transform.m_scale.y};
    
            SDL_RenderCopyEx(renderer, store->GetTexture(sprite.m_spriteId), &sprite.m_srcRect, &dstRect, transform.m_rotation, nullptr, SDL_FLIP_NONE);


            #ifdef DEBUG
                if (entity.HasComponent<BoxColliderComponent>())
                {
                    SDL_SetRenderDrawColor(renderer, 97, 255, 163, 255);
                    auto &boxCollider = entity.GetComponent<BoxColliderComponent>();

                    SDL_Rect boundingBox = {
                        transform.m_position.x + boxCollider.m_offset.x - ((sprite.m_isFixed) ? 0 : camera.x),
                        transform.m_position.y + boxCollider.m_offset.y -((sprite.m_isFixed) ? 0 : camera.y),
                        boxCollider.m_width,
                        boxCollider.m_height};

                    SDL_RenderDrawRect(renderer, &boundingBox);
                }
            #endif
        }
    }
};
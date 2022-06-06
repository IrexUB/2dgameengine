#pragma once

#include <iostream>

#include "../ECS/ECS.h"
#include "../AssetsStore/AssetsStore.h"
#include "../Components/TilemapComponent.h"

class TilemapRenderSystem : public System
{
public:
    TilemapRenderSystem()
    {
        RequireComponent<TilemapComponent>();
    }

    void Update(SDL_Renderer *renderer, AssetsStore *store, SDL_Rect& camera)
    {
        for (auto &entity : GetSystemEntities())
        {
            auto &tilemapComponent = entity.GetComponent<TilemapComponent>();
            Tilemap *tilemap = &tilemapComponent.m_tilemap;
            Tileset *tileset = tilemapComponent.m_tileset.get();

            const auto mapDimensions = tilemap->GetMapDimension();
            for (auto y = 0; y < mapDimensions.m_height; ++y)
            {
                for (auto x = 0; x < mapDimensions.m_width; ++x)
                {
                    auto &[tile, tileNo] = tilemap->GetMapCell(y, x);

                    auto _tile = tile;
                    _tile.x -= camera.x;
                    _tile.y -= camera.y;

                    auto correspondingTile = tileset->Get(tileNo);

                    SDL_RenderCopy(renderer, store->GetTexture(tileset->GetTilesetId()), &correspondingTile, &_tile);
                }
            }
        }
    }
};
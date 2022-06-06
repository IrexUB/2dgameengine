#pragma once

#include <memory>

#include "../Map/Tilemap.h"
#include "../Map/Tileset.h"

struct TilemapComponent
{
    Tilemap m_tilemap;
    std::shared_ptr<Tileset> m_tileset;
    TilemapComponent(std::string mapPath = "", std::shared_ptr<Tileset> tileset = nullptr, double tileScale = 1.0)
        : m_tilemap(mapPath), m_tileset(tileset)
    {
        m_tilemap.LoadMap(m_tileset->GetTileDimension(), tileScale);
    }
};
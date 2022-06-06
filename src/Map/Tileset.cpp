#include <cassert>
#include <iostream>
#include "Tileset.h"

#include "../Utils/Fileutils.h"

Tileset::Tileset(std::string tilesetId)
    : m_tilesetId(tilesetId), m_tileDimension(32, 32)
{
}

void Tileset::Slice(AssetsStore *store, Size2D tileDimension)
{
    int tileSetWidth;
    int tileSetHeight;

    SDL_QueryTexture(store->GetTexture(m_tilesetId), nullptr, nullptr, &tileSetWidth, &tileSetHeight);

    m_tileDimension = std::move(tileDimension);
    const auto cols = static_cast<int>(tileSetWidth / m_tileDimension.m_width);
    const auto rows = static_cast<int>(tileSetHeight / m_tileDimension.m_height);

    m_tiles.clear();
    m_tiles.reserve(rows * cols);

    for (auto y = 0; y < rows; ++y)
    {
        for (auto x = 0; x < cols; ++x)
        {
            Tile tile = {
                x * m_tileDimension.m_width,
                y * m_tileDimension.m_height,
                m_tileDimension.m_width,
                m_tileDimension.m_height,
            };

            m_tiles.push_back(tile);
        }
    }
}

Tile &Tileset::operator[](std::size_t tileNo)
{
    assert(tileNo < m_tiles.size());
    return m_tiles[tileNo];
}

std::size_t Tileset::Size() const
{
    return m_tiles.size();
}

const std::string &Tileset::GetTilesetId() const
{
    return m_tilesetId;
}

const Size2D &Tileset::GetTileDimension() const
{
    return m_tileDimension;
}

Tile &Tileset::Get(std::size_t tileNo)
{
    assert(tileNo < m_tiles.size());
    return m_tiles[tileNo];
}
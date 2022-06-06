#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "../AssetsStore/AssetsStore.h"

using Tile = SDL_Rect;

struct Size2D
{
    uint32_t m_width, m_height;
    Size2D(uint32_t width = 0, uint32_t height = 0)
        : m_width(width), m_height(height)
    {
    }
};

class Tileset
{
public:
    Tileset(std::string tilesetId);
    void Slice(AssetsStore *store, Size2D tileDimension);
    Tile &operator[](std::size_t tileNo);
    Tile &Get(std::size_t tileNo);
    std::size_t Size() const;
    const std::string &GetTilesetId() const;
    const Size2D &GetTileDimension() const;

private:
    std::vector<Tile> m_tiles;
    std::string m_tilesetId;
    Size2D m_tileDimension;
};
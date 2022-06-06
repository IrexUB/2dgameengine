#pragma once

#include <map>
#include <tuple>
#include "Tileset.h"

class Tilemap
{
public:
    Tilemap(std::string mapPath);
    void LoadMap(const Size2D &tileDimension, double tileScale);
    const std::tuple<Tile, int> &GetMapCell(uint32_t row, uint32_t col);
    const Size2D &GetMapDimension() const;

private:
    void FetchMapDimension();

    std::map<std::size_t, std::tuple<Tile, int>> m_tilemap;
    Size2D m_mapDimension;
    std::string m_mapPath;
};
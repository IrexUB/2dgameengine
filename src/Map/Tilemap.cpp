#include <fstream>
#include <sstream>
#include <iostream>

#include "Tilemap.h"
#include "../Logger/Logger.h"

Tilemap::Tilemap(std::string mapPath)
    : m_mapPath(std::move(mapPath))
{
    FetchMapDimension();
}

void Tilemap::LoadMap(const Size2D &tileDimension, double tileScale)
{
    std::ifstream mapFile(m_mapPath);

    m_tilemap.clear();

    for (auto [y, _row] = std::tuple{0, std::string()}; std::getline(mapFile, _row); ++y)
    {
        std::stringstream _stream(_row);
        for (auto [x, _col] = std::tuple{0, std::string()}; std::getline(_stream, _col, ','); ++x)
        {
            auto tileNo = std::atoi(_col.c_str());

            Tile mapTile = {
                (x * tileDimension.m_width) * tileScale,
                (y * tileDimension.m_height) * tileScale,
                tileDimension.m_width * tileScale,
                tileDimension.m_height * tileScale};

            m_tilemap.insert(std::make_pair(m_tilemap.size(), std::make_tuple(std::move(mapTile), tileNo)));
        }
    }

    mapFile.close();
}

void Tilemap::FetchMapDimension()
{
    std::ifstream mapFile(m_mapPath);

    std::string firstRow;
    mapFile >> firstRow;

    uint32_t mapWidth{0};
    for (auto [_stream, _tok] = std::make_tuple(std::stringstream(firstRow), std::string()); std::getline(_stream, _tok, ',');)
        ++mapWidth;

    mapFile.clear();
    mapFile.seekg(0);

    uint32_t mapHeight{0};
    for (std::string _row; std::getline(mapFile, _row);)
        mapHeight++;

    mapFile.close();

    m_mapDimension.m_height = mapHeight;
    m_mapDimension.m_width = mapWidth;
}

const Size2D &Tilemap::GetMapDimension() const
{
    return m_mapDimension;
}
const
std::tuple<Tile, int> &
Tilemap::GetMapCell(uint32_t row, uint32_t col)
{
    const auto cellIt = m_tilemap.find(row * m_mapDimension.m_width + col);

    if (cellIt != m_tilemap.end())
    {
        return cellIt->second;
    }
    else
    {
        Logger::LogError("No cell to this position !");
    }
}
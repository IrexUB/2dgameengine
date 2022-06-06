#include <fstream>
#include <arpa/inet.h>

#include "Fileutils.h"

std::tuple<uint32_t, uint32_t> GetImgDimensions(const std::string &path)
{
    std::ifstream imgFile(path);

    uint32_t width, height;
    imgFile.seekg(16);

    imgFile.read(reinterpret_cast<char *>(&width), 4);
    imgFile.read(reinterpret_cast<char *>(&height), 4);

    width = ntohl(width);
    height = ntohl(height);

    imgFile.close();

    return std::make_tuple(width, height);
}
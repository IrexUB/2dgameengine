#include <SDL2/SDL_image.h>

#include "AssetsStore.h"
#include "../Logger/Logger.h"

inline void AssetsStore::ClearAssets()
{
    m_textures.clear();
}

void AssetsStore::AttachRenderer(SDL_Renderer *renderer)
{
    if (renderer)
        m_renderer = renderer;
    else
        Logger::LogError("Can\'t attach a nullptr renderer");
}

void AssetsStore::AddTexture(std::string textureId, std::string texturePath)
{
    Logger::Log("ENtered in ADdTexture");

    auto textureSurface = IMG_Load(texturePath.c_str());
    if (textureSurface)
    {
        auto texture = std::unique_ptr<SDL_Texture, SDL_Deleter>(
            SDL_CreateTextureFromSurface(m_renderer, textureSurface),
            SDL_Deleter());

        m_textures.insert({std::move(textureId), std::move(texture)});
    }
}

SDL_Texture *AssetsStore::GetTexture(const std::string &textureId) const
{
    auto textureIt = m_textures.find(textureId);
    if (textureIt != m_textures.end())
    {
        return textureIt->second.get();
    }
}
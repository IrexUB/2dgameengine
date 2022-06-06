#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <memory>

#include "../Utils/SDLUtils.h"

class AssetsStore
{
public:
    void AttachRenderer(SDL_Renderer *renderer);
    inline void ClearAssets();
    void AddTexture(std::string textureId, std::string texturePath);
    SDL_Texture *GetTexture(const std::string &textureId) const;

private:
    std::map<std::string, std::unique_ptr<SDL_Texture, SDL_Deleter>> m_textures;
    SDL_Renderer *m_renderer;
};
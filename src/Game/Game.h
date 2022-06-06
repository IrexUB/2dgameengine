#pragma once

#include <SDL2/SDL.h>
#include <memory>

#include "../Utils/SDLUtils.h"
#include "../ECS/ECS.h"
#include "../AssetsStore/AssetsStore.h"
#include "../Events/EventBus.h"

class Game
{
public:
    Game();
    ~Game();
    void Run();

private:
    void InitWindow();
    void InitRenderer();
    void InitAssetsManager();
    void Destroy();
    void Setup();
    void ProcessInput() noexcept;
    void Update() noexcept;
    void Render() noexcept;

    bool m_isRunning;
    uint32_t m_windowHeight;
    uint32_t m_windowWidth;
    uint32_t m_timePreviousFrame = 0;
    std::unique_ptr<SDL_Window, SDL_Deleter> m_window;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> m_renderer;
    std::unique_ptr<Registry> m_registry;
    std::unique_ptr<AssetsStore> m_assetsStore;
    std::unique_ptr<EventBus> m_eventBus;
    SDL_Rect m_camera;
};
#include <iostream>
#include <SDL2/SDL_image.h>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>

#include "Game.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/TilemapComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/BulletEmitterComponent.h"
#include "../Components/RigidBody2D.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/TilemapRenderSystem.h"
#include "../Systems/TilemapRenderSystem.h"
#include "../Systems/Physics2dSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CameraFollowSystem.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../Systems/BulletEmitSystem.h"

#include "../Utils/Fileutils.h"

#include "../Map/Tileset.h"

Game::Game()
    : m_isRunning(false)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw std::runtime_error("SDL_Init() error");
    }

    InitWindow();
    InitRenderer();

    ImGui::CreateContext();
    ImGuiSDL::Initialize(m_renderer.get(), m_windowWidth, m_windowHeight);

    m_registry = std::make_unique<Registry>();
    if (!m_registry)
    {
        throw std::runtime_error("Registry construction error !");
    }

    InitAssetsManager();

    m_eventBus = std::make_unique<EventBus>();

    m_isRunning = true;

    m_camera.x = 0;
    m_camera.y = 0;
    m_camera.w = m_windowWidth;
    m_camera.h = m_windowHeight;

    Logger::Log("Game constructor called !");
}

Game::~Game()
{
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
    Logger::Log("Game destructor called !");
}

void Game::InitWindow()
{
    m_windowWidth = 1920;
    m_windowHeight = 1080;
    m_window = std::unique_ptr<SDL_Window, SDL_Deleter>(
        SDL_CreateWindow(
            "Pixel GE",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_windowWidth,
            m_windowHeight,
            0),
        SDL_Deleter());

    SDL_SetWindowOpacity(m_window.get(), 0.0f);

    if (!m_window)
    {
        throw std::runtime_error("SDL_Window init error");
    }

    Logger::Log("Window constructed !");
}

void Game::InitRenderer()
{
    m_renderer = std::unique_ptr<SDL_Renderer, SDL_Deleter>(
        SDL_CreateRenderer(
            m_window.get(),
            -1,
            SDL_RENDERER_ACCELERATED),
        SDL_Deleter());

    if (!m_renderer)
    {
        throw std::runtime_error("SDL_Renderer init error");
    }

    Logger::Log("Renderer constructed !");
}

void Game::InitAssetsManager()
{
    m_assetsStore = std::make_unique<AssetsStore>();
    if (!m_assetsStore)
    {
        throw std::runtime_error("AssetsStore init error");
    }

    m_assetsStore->AttachRenderer(m_renderer.get());
}

void Game::Destroy()
{
    SDL_Quit();
}

void Game::ProcessInput() noexcept
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        ImGuiIO& io = ImGui::GetIO();
        
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        switch (event.type)
        {
        case SDL_QUIT:
            m_isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
            }
            m_eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym);
            break;
        }
    }
}

void Game::Setup()
{
    m_registry->AddSystem<MovementSystem>();
    m_registry->AddSystem<RenderSystem>();
    m_registry->AddSystem<TilemapRenderSystem>();
    m_registry->AddSystem<Physics2dSystem>();
    m_registry->AddSystem<CameraFollowSystem>();
    m_registry->AddSystem<AnimationSystem>();
    m_registry->AddSystem<KeyboardMovementSystem>();
    m_registry->AddSystem<BulletEmitSystem>();
    m_registry->AddSystem<RenderGUISystem>();

    m_registry->GetSystem<MovementSystem>().SubscribeToEvents(m_eventBus.get());
    m_registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(m_eventBus.get());

    m_assetsStore->AddTexture("default-sprite", "assets/images/default.png");
    m_assetsStore->AddTexture("tank-right", "assets/images/tank-panther-right.png");
    m_assetsStore->AddTexture("truck-right", "assets/images/truck-ford-right.png");
    m_assetsStore->AddTexture("chopper", "assets/images/chopper-spritesheet.png");
    m_assetsStore->AddTexture("landing-base", "assets/images/landing-base.png");
    m_assetsStore->AddTexture("jungle-tileset", "assets/tilemaps/jungle.png");
    m_assetsStore->AddTexture("bullet", "assets/images/bullet.png");

    std::shared_ptr<Tileset> jungleTileset = std::make_shared<Tileset>("jungle-tileset");
    jungleTileset->Slice(m_assetsStore.get(), {32, 32});


    Entity tank = m_registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(100, 100), glm::vec2(2, 2), 0);
    tank.AddComponent<RigidBody2D>(glm::vec2(30, 0));
    tank.AddComponent<SpriteComponent>("tank-right", SDL_Rect(0, 0, 32, 32), 1);
    tank.AddComponent<BoxColliderComponent>(64, 64);
    tank.AddComponent<BulletEmitterComponent>(glm::vec2(100, 0), 5000);


    Entity chopper = m_registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2(300, 400), glm::vec2(2, 2), 0);
    chopper.AddComponent<RigidBody2D>(glm::vec2(0, 0));
    chopper.AddComponent<SpriteComponent>("chopper", SDL_Rect(0, 0, 32, 32), 1);
    chopper.AddComponent<AnimationComponent>(2, 10);
    chopper.AddComponent<BoxColliderComponent>(64, 64);
    chopper.AddComponent<KeyboardMovementComponent>(200);
    chopper.AddComponent<CameraFollowComponent>();

    Entity landing = m_registry->CreateEntity();
    landing.AddComponent<TransformComponent>(glm::vec2(800, 100), glm::vec2(2, 2), 0);
    landing.AddComponent<SpriteComponent>("landing-base", SDL_Rect(0, 0, 32, 32), 0);
    landing.AddComponent<BoxColliderComponent>(64, 64);

    Entity tilemap = m_registry->CreateEntity();
    tilemap.AddComponent<TilemapComponent>("assets/tilemaps/jungle.map", jungleTileset, 2.5);
}

void Game::Update() noexcept
{
    double deltaTime = (SDL_GetTicks() - m_timePreviousFrame) / 1000.0f;

    m_registry->Update();

    m_registry->GetSystem<CameraFollowSystem>().Update(m_camera);
    m_registry->GetSystem<MovementSystem>().Update(deltaTime);
    m_registry->GetSystem<AnimationSystem>().Update();
    m_registry->GetSystem<Physics2dSystem>().Update(m_eventBus.get());
    m_registry->GetSystem<BulletEmitSystem>().Update(m_registry);

    m_timePreviousFrame = SDL_GetTicks();
}

void Game::Render() noexcept
{
    auto renderer = m_renderer.get();

    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderClear(renderer);

    m_registry->GetSystem<TilemapRenderSystem>().Update(renderer, m_assetsStore.get(), m_camera);
    m_registry->GetSystem<RenderSystem>().Update(renderer, m_assetsStore.get(), m_camera);
    m_registry->GetSystem<RenderGUISystem>().Update(m_registry);

    SDL_RenderPresent(renderer);
}

void Game::Run()
{
    Setup();

    while (m_isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }

    Destroy();
}
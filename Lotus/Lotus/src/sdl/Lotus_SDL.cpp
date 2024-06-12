#include "Lotus_SDL.h"
#include "../log/Lotus_Log.h"
#include "../ecs/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../systems/MovementSystem.h"
#include "../systems/RenderSystem.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm.hpp>
#include <iostream>
using namespace std;
Lotus_SDL::Lotus_SDL()
{
    registry = std::make_unique<EntityManager>();
    Lotus_Log::Info("LotusSDL constructor called");
}

Lotus_SDL::~Lotus_SDL()
{
    Lotus_Log::Info("LotusSDL destructor called");
}

void Lotus_SDL::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Lotus_Log::Error("Error initializing SDL!!!");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    window = SDL_CreateWindow("Lotus",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        Lotus_Log::Error("Error creating SDL window!!!");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        Lotus_Log::Error("Error creating SDL renderer!!!");
        return;
    }
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void Lotus_SDL::Run()
{
    Setup();
    while (!quit)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Lotus_SDL::Setup()
{
    // Add the systems that need to be processed in our game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    // Create some entities
    Entity tank = registry->CreateEntity();
    Entity truck = registry->CreateEntity();
    // Add components to entity
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(1.0, 1.0));
    tank.AddComponent<SpriteComponent>(10, 10);
    //tank.RemoveComponent<TransformComponent>();
    // tank.AddComponent<TransformComponent>();
    // tank.AddComponent<BoxColliderComponent>();
    // tank.AddComponent<SpriteComponent>("./art/......."/tank.png);
}

void Lotus_SDL::ProcessInput()
{
    SDL_Event currentEvents;
    // Check for events
    while (SDL_PollEvent(&currentEvents))
    {
        switch (currentEvents.type)
        {
            case SDL_QUIT:
            {
                quit = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (currentEvents.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        quit = true;
                        break;
                    }
                }
            }
        }
    }
}

void Lotus_SDL::Update()
{
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
    {
        SDL_Delay(timeToWait);
    }
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    millisecsPreviousFrame = SDL_GetTicks();
    
    // Update the registry to process the entities that are waiting to be created or removed
    registry->Update();

    // Update the systems
    registry->GetSystem<MovementSystem>().Update(deltaTime);


    // CollisionSystem.Update();
    // DamageSystem.Update();
}

void Lotus_SDL::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer);
    SDL_RenderPresent(renderer);
}

void Lotus_SDL::OnDestroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
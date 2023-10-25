#include "Lotus_SDL.h"
#include "../log/Lotus_Log.h"
#include "SDL_image.h"
#include <glm.hpp>
#include <iostream>
using namespace std;
Lotus_SDL::Lotus_SDL()
{
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
    // TODO: 
    // Entity tank = EntityManager.CreateEntity();
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
    float deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    millisecsPreviousFrame = SDL_GetTicks();
    // TODO: 
    // MovementSystem.Update();
    // CollisionSystem.Update();
    // DamageSystem.Update();
}

void Lotus_SDL::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    SDL_RenderPresent(renderer);
}

void Lotus_SDL::OnDestroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
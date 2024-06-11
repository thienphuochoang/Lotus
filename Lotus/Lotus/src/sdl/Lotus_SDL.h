#pragma once
#include <SDL.h>
#include "../managers/EntityManager.h"
#include <memory>
#define FPS 60
#define MILLISECS_PER_FRAME (1000/FPS)
class Lotus_SDL
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int millisecsPreviousFrame = 0;
    bool quit = false;
    std::unique_ptr<EntityManager> registry;
public:
    Lotus_SDL();
    ~Lotus_SDL();
    void Initialize();
    void Run();
    void ProcessInput();
    void Setup();
    void Update();
    void Render();
    void OnDestroy();
};


#pragma once
#include <SDL.h>
#define FPS 60
#define MILLISECS_PER_FRAME (1000/FPS)
class Lotus_SDL
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int millisecsPreviousFrame = 0;
    bool quit = false;
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


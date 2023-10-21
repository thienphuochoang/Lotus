#pragma once
#include <SDL.h>
#define FPS 60
#define MILLISECS_PER_FRAME (1000/FPS)
class Game
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int millisecsPreviousFrame = 0;
    bool quit = false;
public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void ProcessInput();
    void Setup();
    void Update();
    void Render();
    void OnDestroy();
};


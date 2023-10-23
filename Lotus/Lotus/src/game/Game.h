#pragma once
#include <SDL.h>
#include "../sdl/Lotus_SDL.h"
#define FPS 60
#define MILLISECS_PER_FRAME (1000/FPS)
class Game
{
private:
    Game();
    ~Game();
    Lotus_SDL* lotusSDL;
    static Game* instance;
public:
    void Initialize();
    void Run();
    void ProcessInput();
    void Setup();
    void Update();
    void Render();
    void OnDestroy();
    static Game* GetInstance();
};


#include <iostream>
#include "./src/sdl/Lotus_SDL.h"
int main(int argc, char* argv[])
{
    std::unique_ptr<Lotus_SDL> game = std::make_unique<Lotus_SDL>();
    game->Initialize();
    game->Run();
    game->OnDestroy();
    return 0;
}
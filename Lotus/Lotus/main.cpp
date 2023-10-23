#include <iostream>
#include "./src/game/Game.h"
int main(int argc, char* argv[])
{
    Game* game;
    game = Game::GetInstance();
    game->Initialize();
    game->Run();
    game->OnDestroy();
    return 0;
}
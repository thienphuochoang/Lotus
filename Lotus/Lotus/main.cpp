#include <iostream>
#include "Game.h"
int main(int argc, char* argv[])
{
    Game game;
    game.Initialize();
    game.Run();
    game.OnDestroy();
    return 0;
}
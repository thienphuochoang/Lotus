#include "./src/sdl/Lotus_SDL.h"
#include <sol.hpp>
#include <iostream>
void TestLua()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file("./scripts/myscript.lua");

    int someVariableInsideCpp = lua["some_variable"];
    std::cout << "The value of some_variable is " << someVariableInsideCpp << std::endl;
}
int main(int argc, char* argv[])
{
    std::unique_ptr<Lotus_SDL> game = std::make_unique<Lotus_SDL>();
    game->Initialize();
    game->Run();
    game->OnDestroy();
    //TestLua();
    return 0;
}
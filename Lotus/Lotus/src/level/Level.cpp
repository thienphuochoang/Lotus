#include "Level.h"
#include "../sdl/Lotus_SDL.h"
#include <fstream>
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/AnimationComponent.h"
#include "../components/BoxColliderComponent.h"
#include "../components/KeyboardControlComponent.h"
#include "../components/CameraComponent.h"
#include "../components/ProjectileEmitterComponent.h"
#include "../components/HealthComponent.h"
#include "../components/TextComponent.h"
#include <sol.hpp>

void Level::Load(const std::unique_ptr<EntityManager>& registry,
    const std::unique_ptr<Asset>& assets,
    SDL_Renderer* renderer,
    unsigned int level)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    // Adding assets
    assets->AddTexture(renderer, "soldier1-image", "./assets/images/soldier1.png");
    assets->AddTexture(renderer, "soldier2-image", "./assets/images/soldier2.png");
    assets->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    assets->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
    assets->AddTexture(renderer, "container-image", "./assets/images/Container_A.png");
    assets->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 20);
    assets->AddFont("pico8-font", "./assets/fonts/pico8.ttf", 10);

    // Load the tilemap
    int tileSize = 32;
    double tileScale = 2.0;
    int mapNumCols = 25;
    int mapNumRows = 20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");
    for (int y = 0; y < mapNumRows; y++)
    {
        for (int x = 0; x < mapNumCols; x++)
        {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.Group("tiles");
            tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize),
                y * (tileScale * tileSize)),
                glm::vec2(tileScale, tileScale),
                0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", 0, false, tileSize, tileSize, srcRectX, srcRectY);
        }
    }
    mapFile.close();
    Lotus_SDL::MAP_WIDTH = mapNumCols * tileSize * tileScale;
    Lotus_SDL::MAP_HEIGHT = mapNumRows * tileSize * tileScale;

    // Create some entities
    Entity soldier = registry->CreateEntity();
    // Add components to entity
    soldier.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    soldier.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    soldier.AddComponent<SpriteComponent>("soldier1-image", 1, false, 64, 64);
    soldier.AddComponent<AnimationComponent>(8, 10, true);
    soldier.AddComponent<BoxColliderComponent>(32, 32, glm::vec2(16, 16));
    soldier.AddComponent<KeyboardControlComponent>(glm::vec2(0, -100),
        glm::vec2(0, 100),
        glm::vec2(-100, 0),
        glm::vec2(100, 0));
    soldier.AddComponent<CameraComponent>();
    soldier.AddComponent<HealthComponent>(100);
    soldier.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 5000, 10, true);
    soldier.Tag("player");

    // Create some entities
    Entity soldier2 = registry->CreateEntity();
    // Add components to entity
    soldier2.AddComponent<TransformComponent>(glm::vec2(500.0, 200.0), glm::vec2(1.0, 1.0), -90.0);
    soldier2.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
    soldier2.AddComponent<SpriteComponent>("soldier2-image", 2, false, 64, 64);
    soldier2.AddComponent<AnimationComponent>(8, 10, true);
    soldier2.AddComponent<BoxColliderComponent>(32, 32, glm::vec2(16, 16));
    soldier2.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 1000, 50, false);
    soldier2.AddComponent<HealthComponent>(100);
    soldier2.Group("enemies");

    Entity container = registry->CreateEntity();
    container.AddComponent<TransformComponent>(glm::vec2(800.0, 200.0), glm::vec2(1.0, 1.0), 0.0);
    container.AddComponent<SpriteComponent>("container-image", 2, false, 256, 128);
    container.AddComponent<BoxColliderComponent>(256, 128);
    container.Group("obstacles");

    Entity container2 = registry->CreateEntity();
    container2.AddComponent<TransformComponent>(glm::vec2(200.0, 200.0), glm::vec2(1.0, 1.0), 0.0);
    container2.AddComponent<SpriteComponent>("container-image", 2, false, 256, 128);
    container2.AddComponent<BoxColliderComponent>(256, 128);
    container2.Group("obstacles");
}

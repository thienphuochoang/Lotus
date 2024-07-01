#pragma once
#include "../ecs/ECS.h"
#include "../assets/Asset.h"
class Level
{
public:
    Level() = default;
    ~Level() = default;
    void Load(const std::unique_ptr<EntityManager>& registry, 
        const std::unique_ptr<Asset>& assets,
        SDL_Renderer* renderer,
        unsigned int level);
};
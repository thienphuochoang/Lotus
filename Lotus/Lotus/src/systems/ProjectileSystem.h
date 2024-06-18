#pragma once
#include "../ecs/ECS.h"
#include "../components/ProjectileComponent.h"

class ProjectileSystem : public System
{
public:
    ProjectileSystem()
    {
        RequireComponent<ProjectileComponent>();
    }
    void Update()
    {
        for (auto entity : GetSystemEntities())
        {
            auto projectile = entity.GetComponent<ProjectileComponent>();
            if (SDL_GetTicks() - projectile.startTime > projectile.duration)
            {
                entity.Destroy();
            }
        }
    }
};
#pragma once
#include "../ecs/ECS.h"
#include "../components/BoxColliderComponent.h"
#include "../components/TransformComponent.h"
#include <SDL.h>
class RenderColliderSystem : public System
{
public:
    RenderColliderSystem()
    {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }
    void Update(SDL_Renderer* renderer)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto transformComp = entity.GetComponent<TransformComponent>();
            const auto boxColliderComp = entity.GetComponent<BoxColliderComponent>();

            SDL_Rect colliderRect =
            {
                static_cast<int>(transformComp.position.x + boxColliderComp.offset.x),
                static_cast<int>(transformComp.position.y + boxColliderComp.offset.y),
                static_cast<int>(boxColliderComp.width),
                static_cast<int>(boxColliderComp.height)
            };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
    }
};
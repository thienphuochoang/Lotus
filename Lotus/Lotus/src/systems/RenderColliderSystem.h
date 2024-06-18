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
    void Update(SDL_Renderer* renderer, SDL_Rect& camera)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto transformComp = entity.GetComponent<TransformComponent>();
            const auto boxColliderComp = entity.GetComponent<BoxColliderComponent>();

            SDL_Rect colliderRect =
            {
                static_cast<int>(transformComp.position.x + boxColliderComp.offset.x - camera.x),
                static_cast<int>(transformComp.position.y + boxColliderComp.offset.y - camera.y),
                static_cast<int>(boxColliderComp.width * transformComp.scale.x),
                static_cast<int>(boxColliderComp.height * transformComp.scale.y)
            };
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
    }
};
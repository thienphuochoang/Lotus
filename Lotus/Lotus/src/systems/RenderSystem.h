#pragma once
#include "../ecs/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include <SDL.h>
class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }
    void Update(SDL_Renderer* renderer)
    {
        // Loop all entities that the system is interested in 
        for (auto entity : GetSystemEntities())
        {
            // Update entity position based on its velocity
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto sprite = entity.GetComponent<SpriteComponent>();

            SDL_Rect objRect = { static_cast<int>(transform.position.x), 
                static_cast<int>(transform.position.y),
                sprite.width, 
                sprite.height };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &objRect);

            Lotus_Log::Info("Entity id = " + std::to_string(entity.GetId()) +
                " position is " + std::to_string(transform.position.x) + ", " +
                std::to_string(transform.position.y));
        }
    }
};
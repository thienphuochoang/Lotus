#pragma once
#include "../ecs/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include <SDL.h>
#include <algorithm>
class RenderSystem : public System
{
public:
    RenderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
    }
    void Update(SDL_Renderer* renderer, std::unique_ptr<Asset>& assets, SDL_Rect& camera)
    {
        // Create a temporary vector storing both TransformComponent and SpriteComponent of 
        // all entities
        struct RenderEntity
        {
            TransformComponent transformComponent;
            SpriteComponent spriteComponent;
        };
        std::vector<RenderEntity> renderEntities;
        
        for (auto entity : GetSystemEntities())
        {
            RenderEntity renderEntity;
            renderEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
            renderEntity.transformComponent = entity.GetComponent<TransformComponent>();
            renderEntities.emplace_back(renderEntity);
        }

        // Sort all the entities of our system by zIndex
        std::sort(renderEntities.begin(), renderEntities.end(), [](const RenderEntity& a, const RenderEntity& b)
        {
            return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
        });

        // Loop all entities that the system is interested in 
        for (auto entity : renderEntities)
        {
            const auto transform = entity.transformComponent;
            const auto sprite = entity.spriteComponent;

            // Set the source rectangle of original sprite texture
            SDL_Rect srcRect = sprite.srcRect;

            // Set the destination rectangle with the x,y position to be rendered
            SDL_Rect destRect =
            {
                static_cast<int>(transform.position.x - (sprite.isFixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y - (sprite.isFixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            SDL_RenderCopyEx(renderer, 
                assets->GetTexture(sprite.assetId), 
                &srcRect, 
                &destRect, 
                transform.rotation,
                NULL,
                SDL_FLIP_NONE);
        }
    }
};
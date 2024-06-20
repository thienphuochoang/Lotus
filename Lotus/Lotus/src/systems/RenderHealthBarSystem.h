#pragma once
#include <SDL.h>
#include "../ecs/ECS.h"
#include "../assets/Asset.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/HealthComponent.h"
class RenderHealthBarSystem : public System
{
public:
    RenderHealthBarSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<HealthComponent>();
    }

    void Update(SDL_Renderer* renderer, std::unique_ptr<Asset>& assets, const SDL_Rect& camera)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto transformComp = entity.GetComponent<TransformComponent>();
            const auto spriteComp = entity.GetComponent<SpriteComponent>();
            const auto healthComp = entity.GetComponent<HealthComponent>();

            // Draw a health bar
            SDL_Color healthBarColor = { 255, 255, 255 };
            if (healthComp.health >= 0 && healthComp.health < 40)
            {
                healthBarColor = { 255, 0, 0 };
            }
            else if (healthComp.health >= 40 && healthComp.health < 80)
            {
                healthBarColor = { 255, 255, 0 };
            }
            else if (healthComp.health >= 80 && healthComp.health <= 100)
            {
                healthBarColor = { 0, 255, 0 };
            }

            // Position the health bar indicator on the top of the entity's sprite
            int healthBarWidth = 15;
            int healthBarHeight = 3;
            double healthBarPosX = transformComp.position.x + spriteComp.width / 4 - camera.x;
            double healthBarPosY = transformComp.position.y - camera.y;

            SDL_Rect healthBarRect =
            {
                static_cast<int>(healthBarPosX),
                static_cast<int>(healthBarPosY),
                static_cast<int>(healthBarWidth * (healthComp.health / 100.0)),
                static_cast<int>(healthBarHeight)
            };

            SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, 255);
            SDL_RenderFillRect(renderer, &healthBarRect);

            // Render the health text
            std::string healthText = std::to_string(healthComp.health);
            SDL_Surface* surface = TTF_RenderText_Blended(assets->GetFont("pico8-font"), healthText.c_str(), healthBarColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int labelWidth = 0;
            int labelHeight = 0;
            SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);
            SDL_Rect healthBarTextRect =
            {
                static_cast<int>(healthBarPosX),
                static_cast<int>(healthBarPosY) + 5,
                labelWidth,
                labelHeight
            };

            SDL_RenderCopy(renderer, texture, NULL, &healthBarTextRect);
            SDL_DestroyTexture(texture);
        }
    }
};
#pragma once
#include "../ecs/ECS.h"
#include "../components/TextComponent.h"
#include "../assets/Asset.h"
#include <SDL.h>

class RenderTextSystem : public System
{
public:
    RenderTextSystem()
    {
        RequireComponent<TextComponent>();
    }
    void Update(SDL_Renderer* renderer, std::unique_ptr<Asset>& asset, const SDL_Rect& camera)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto textComp = entity.GetComponent<TextComponent>();
            SDL_Surface* surface = TTF_RenderText_Blended(asset->GetFont(textComp.assetId),
                textComp.text.c_str(),
                textComp.color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int textWidth = 0;
            int textHeight = 0;

            SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
            SDL_Rect dstRect =
            {
                static_cast<int>(textComp.position.x - (textComp.isFixed ? 0 : camera.x)),
                static_cast<int>(textComp.position.y - (textComp.isFixed ? 0 : camera.y)),
                textWidth,
                textHeight
            };

            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_DestroyTexture(texture);
        }
    }
};
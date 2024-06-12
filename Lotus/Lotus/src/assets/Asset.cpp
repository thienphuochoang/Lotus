#include "Asset.h"
#include "../log/Lotus_Log.h"
#include <SDL_image.h>

Asset::Asset()
{
    Lotus_Log::Info("Asset Constructor called");
}

Asset::~Asset()
{
    ClearAssets();
    Lotus_Log::Info("Asset Destructor called");
}

void Asset::ClearAssets()
{
    for (auto texture : textures)
    {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void Asset::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);
}

SDL_Texture* Asset::GetTexture(const std::string& assetId)
{
    return textures[assetId];
}

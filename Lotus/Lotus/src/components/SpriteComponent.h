#pragma once
#include <glm.hpp>
#include <SDL.h>
struct SpriteComponent
{
    std::string assetId;
    int width;
    int height;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    int zIndex;
    SpriteComponent(std::string assetId = "", int zIndex = 0, int width = 0, int height = 0,
        int srcRectX = 0, int srcRextY = 0)
        : assetId(assetId), width(width), height(height), zIndex(zIndex)
    {
        srcRect = { srcRectX, srcRextY, width, height };
    };
};
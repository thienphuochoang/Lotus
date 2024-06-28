#pragma once
#include <glm.hpp>
#include <SDL.h>
struct SpriteComponent
{
    std::string assetId;
    int width;
    int height;
    SDL_Rect srcRect;
    int zIndex;
    bool isFixed;
    SDL_RendererFlip flip;
    SpriteComponent(std::string assetId = "", int zIndex = 0, bool isFixed = false, int width = 0, int height = 0,
        int srcRectX = 0, int srcRextY = 0)
        : assetId(assetId), width(width), height(height), zIndex(zIndex), isFixed(isFixed)
    {
        srcRect = { srcRectX, srcRextY, width, height };
        flip = SDL_FLIP_NONE;
    };
};
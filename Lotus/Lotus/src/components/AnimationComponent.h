#pragma once
#include <SDL.h>

struct AnimationComponent
{
    int numFrames;
    int currentFrame;
    int frameSpeedRate;
    bool isLoop;
    int startTime;

    AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool isLoop = true) :
        numFrames(numFrames), frameSpeedRate(frameSpeedRate), isLoop(isLoop)
    {
        currentFrame = 1;
        startTime = SDL_GetTicks();
    }
};
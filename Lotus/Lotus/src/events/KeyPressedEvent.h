#pragma once
#include "../ecs/ECS.h"
#include "../events/EventManager.h"
#include <SDL.h>
class KeyPressedEvent : public Event
{
public:
    SDL_Keycode keycode;
    KeyPressedEvent(SDL_Keycode keycode) : keycode(keycode)
    {

    }
};
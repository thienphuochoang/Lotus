#pragma once
#include "../ecs/ECS.h"
#include "../components/CameraComponent.h"
#include "../components/TransformComponent.h"
#include <SDL.h>

class CameraMovementSystem : public System
{
public:
    CameraMovementSystem()
    {
        RequireComponent<CameraComponent>();
        RequireComponent<TransformComponent>();
    }
    void Update(SDL_Rect& camera)
    {
        for (auto entity : GetSystemEntities())
        {
            auto transformComp = entity.GetComponent<TransformComponent>();

            if (transformComp.position.x + (camera.w / 2) < Lotus_SDL::MAP_WIDTH)
            {
                camera.x = transformComp.position.x - (Lotus_SDL::WINDOW_WIDTH / 2);
            }

            if (transformComp.position.y + (camera.h / 2) < Lotus_SDL::MAP_HEIGHT)
            {
                camera.y = transformComp.position.y - (Lotus_SDL::WINDOW_HEIGHT / 2);
            }

            // Keep camera view inside the screen limits
            camera.x = camera.x < 0 ? 0 : camera.x;
            camera.y = camera.y < 0 ? 0 : camera.y;
            camera.x = camera.x > camera.w ? camera.w : camera.x;
            camera.y = camera.y > camera.h ? camera.h : camera.y;

            Lotus_Log::Info("Camera changed its position to: " + std::to_string(camera.x) + " " + std::to_string(camera.y));
        }
    }
};
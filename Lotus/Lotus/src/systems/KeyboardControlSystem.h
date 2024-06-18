#pragma once
#include "../ecs/ECS.h"
#include "../events/EventManager.h"
#include "../events/KeyPressedEvent.h"
#include "../components/KeyboardControlComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/TransformComponent.h"

class KeyboardControlSystem : public System
{
public:
    KeyboardControlSystem()
    {
        RequireComponent<KeyboardControlComponent>();
        RequireComponent<RigidBodyComponent>();
        RequireComponent<TransformComponent>();
    }
    void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
    {
        eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event)
    {
        for (auto entity : GetSystemEntities())
        {
            const auto keyboardControlComp = entity.GetComponent<KeyboardControlComponent>();
            auto& rigidBodyComp = entity.GetComponent<RigidBodyComponent>();
            auto& transformComp = entity.GetComponent<TransformComponent>();
            switch (event.keycode)
            {
                case SDLK_UP:
                {
                    rigidBodyComp.velocity = keyboardControlComp.upVelocity;
                    transformComp.rotation = 180;
                    break;
                }
                case SDLK_DOWN:
                {
                    rigidBodyComp.velocity = keyboardControlComp.downVelocity;
                    transformComp.rotation = 0;
                    break;
                }
                case SDLK_LEFT:
                {
                    rigidBodyComp.velocity = keyboardControlComp.leftVelocity;
                    transformComp.rotation = 90;
                    break;
                }
                case SDLK_RIGHT:
                {
                    rigidBodyComp.velocity = keyboardControlComp.rightVelocity;
                    transformComp.rotation = -90;
                    break;
                }
            }
        }
    }
};
#pragma once
#include "../ecs/ECS.h"
#include "../events/EventManager.h"
#include "../events/KeyPressedEvent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"

class KeyboardControlSystem : public System
{
public:
    KeyboardControlSystem() = default;
    void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
    {
        eventManager->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event)
    {
        std::string key = std::to_string(event.keycode);
        std::string keySymbol(1, event.keycode);
        Lotus_Log::Info("Key pressed event executed!! " + key + " " + keySymbol);
    }
};
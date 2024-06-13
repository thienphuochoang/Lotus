#pragma once
#include "../ecs/ECS.h"
#include "../components/BoxColliderComponent.h"
#include "../events/CollisionEvent.h"
#include "../events/EventManager.h"

class DamageSystem : public System
{
public:
    DamageSystem()
    {
        RequireComponent<BoxColliderComponent>();
    }
    void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
    {
        eventManager->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
    }
    void onCollision(CollisionEvent& event)
    {
        Lotus_Log::Info("Event triggered between 2 entities, entity " + std::to_string(event.a.GetId()) + " and entity " + std::to_string(event.b.GetId()));
        event.a.Destroy();
        event.b.Destroy();
    }
};
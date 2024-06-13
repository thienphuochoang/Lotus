#pragma once
#include "../ecs/ECS.h"
#include "../components/BoxColliderComponent.h"
#include "../components/TransformComponent.h"
#include "../events/EventManager.h"
#include "../events/CollisionEvent.h"
class CollisionSystem : public System
{
public:
    CollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }
    void Update(std::unique_ptr<EventManager>& eventManager)
    {
        auto entities = GetSystemEntities();

        // Loop all the entities that the system is interested in
        for (auto i = entities.begin(); i != entities.end(); i++)
        {
            Entity a = *i;
            auto aTransform = a.GetComponent<TransformComponent>();
            auto aCollider = a.GetComponent<BoxColliderComponent>();

            // Loop all the entities that still need to be checked (to the right of i)
            for (auto j = i; j != entities.end(); j++)
            {
                Entity b = *j;

                // Skip if a and b are the same entity
                if (a == b) continue;
                auto bTransform = b.GetComponent<TransformComponent>();
                auto bCollider = b.GetComponent<BoxColliderComponent>();

                // AABB collision check
                bool IsColliding = CheckAABBCollision(
                    aTransform.position.x + aCollider.offset.x,
                    aTransform.position.y + aCollider.offset.y,
                    aCollider.width,
                    aCollider.height,
                    bTransform.position.x + bCollider.offset.x,
                    bTransform.position.y + bCollider.offset.y,
                    bCollider.width,
                    bCollider.height
                );
                if (IsColliding)
                {
                    Lotus_Log::Info("Entity " + std::to_string(a.GetId()) + " is colliding with entity " + std::to_string(b.GetId()));
                    eventManager->EmitEvent<CollisionEvent>(a, b);
                }
            }
        }
    }
    bool CheckAABBCollision(double aTransformX, double aTransformY,
        double aColliderW, double aColliderH,
        double bTransformX, double bTransformY,
        double bColliderW, double bColliderH)
    {
        return
        {
            aTransformX < bTransformX + bColliderW &&
            aTransformX + aColliderW > bTransformX &&
            aTransformY < bTransformY + bColliderH &&
            aTransformY + aColliderH > bTransformY
        };
    }
};
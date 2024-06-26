#pragma once
#include "../ecs/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"

class MovementSystem : public System
{
public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }
    void Update(double deltaTime)
    {
        // Loop all entities that the system is interested in 
        for (auto entity : GetSystemEntities())
        {
            // Update entity position based on its velocity
            auto& transform = entity.GetComponent<TransformComponent>();
            const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
            transform.position.x += rigidbody.velocity.x * deltaTime;
            transform.position.y += rigidbody.velocity.y * deltaTime;

            bool isEntityOutsideMap = (transform.position.x < 0 ||
                transform.position.x > Lotus_SDL::MAP_WIDTH ||
                transform.position.y < 0 ||
                transform.position.x > Lotus_SDL::MAP_HEIGHT);

            // Remove any entities that moves outside the map boundaries
            if (isEntityOutsideMap && !entity.HasTag("player"))
            {
                entity.Destroy();
            }
        }
    }

};
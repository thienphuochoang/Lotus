#pragma once
#include "../ecs/ECS.h"
#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"
#include "../components/SpriteComponent.h"
#include "../events/CollisionEvent.h"
#include "../events/EventManager.h"

class MovementSystem : public System
{
public:
    MovementSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }
    void SubscribeToEvents(std::unique_ptr<EventManager>& eventManager)
    {
        eventManager->SubscribeToEvent<CollisionEvent>(this, &MovementSystem::onCollision);
    }
    void onCollision(CollisionEvent& event)
    {
        Entity a = event.a;
        Entity b = event.b;

        if (a.BelongsToGroup("enemies") && b.BelongsToGroup("obstacles"))
        {
            OnEnemyHitObstacles(a, b);
        }

        if (b.BelongsToGroup("enemies") && a.BelongsToGroup("obstacles"))
        {
            OnEnemyHitObstacles(b, a);
        }
    }
    void OnEnemyHitObstacles(Entity enemy, Entity obstacle)
    {
        if (enemy.HasComponent<RigidBodyComponent>() && enemy.HasComponent<SpriteComponent>())
        {
            auto& spriteComp = enemy.GetComponent<SpriteComponent>();
            auto& rigidBodyComp = enemy.GetComponent<RigidBodyComponent>();
            auto& transformComp = enemy.GetComponent<TransformComponent>();
            if (rigidBodyComp.velocity.x != 0)
            {
                rigidBodyComp.velocity.x *= -1;
                //transformComp.rotation *= -1;
                spriteComp.flip = (spriteComp.flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
            }
            if (rigidBodyComp.velocity.y != 0)
            {
                rigidBodyComp.velocity.y *= -1;
                //transformComp.rotation *= -1;
                spriteComp.flip = (spriteComp.flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            }
        }
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

            // Prevent the player from moving outside the map
            if (entity.HasTag("player"))
            {
                int paddingLeft = 5;
                int paddingRight = 5;
                int paddingTop = 5;
                int paddingBottom = 5;
                transform.position.x = transform.position.x < paddingLeft ? paddingLeft : transform.position.x;
                transform.position.x = transform.position.x > Lotus_SDL::MAP_WIDTH - paddingRight ? Lotus_SDL::MAP_WIDTH - paddingRight : transform.position.x;
                transform.position.y = transform.position.y < paddingTop ? paddingTop : transform.position.y;
                transform.position.y = transform.position.y > Lotus_SDL::MAP_HEIGHT - paddingBottom ? Lotus_SDL::MAP_HEIGHT - paddingBottom : transform.position.y;
            }

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
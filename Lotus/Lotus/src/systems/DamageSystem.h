#pragma once
#include "../ecs/ECS.h"
#include "../components/BoxColliderComponent.h"
#include "../components/ProjectileComponent.h"
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
        Entity a = event.a;
        Entity b = event.b;
        Lotus_Log::Info("Event triggered between 2 entities, entity " + std::to_string(event.a.GetId()) + " and entity " + std::to_string(event.b.GetId()));

        if (a.BelongsToGroup("projectiles") && b.HasTag("player"))
        {
            OnProjectileHitPlayer(a, b);
        }

        if (b.BelongsToGroup("projectiles") && a.HasTag("player"))
        {
            OnProjectileHitPlayer(b, a);
        }

        if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies"))
        {
            OnProjectileHitEnemy(a, b);
        }

        if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies"))
        {
            OnProjectileHitEnemy(b, a);
        }
    }
    void OnProjectileHitPlayer(Entity projectile, Entity player)
    {
        auto projectileComp = projectile.GetComponent<ProjectileComponent>();
        if (!projectileComp.isFriendly)
        {
            // Get the health component of the player
            auto& health = player.GetComponent<HealthComponent>();

            // Reduce the player's health
            health.health -= projectileComp.hitPercentDamage;

            if (health.health <= 0)
            {
                player.Destroy();
            }
            projectile.Destroy();
        }
    }

    void OnProjectileHitEnemy(Entity projectile, Entity enemy)
    {
        const auto projectileComp = projectile.GetComponent<ProjectileComponent>();
        if (projectileComp.isFriendly)
        {
            // Get the health component of the enemy
            auto& health = enemy.GetComponent<HealthComponent>();

            // Reduce the enemy's health
            health.health -= projectileComp.hitPercentDamage;

            if (health.health <= 0)
            {
                enemy.Destroy();
            }
            projectile.Destroy();
        }
    }
};
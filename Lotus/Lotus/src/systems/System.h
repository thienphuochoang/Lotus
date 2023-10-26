#pragma once
#include <vector>
#include "../entities/Entity.h"
#include "../ecs/ECS.h"
// System
// The system processes entities that contain a specific signature
class System
{
    private:
        Signature componentSignature;
        std::vector<Entity> entities;
    public:
        System() = default;
        ~System() = default;
        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        // Define the component type T that entities must have to be
        // considered by the system
        template <typename TComponent> void RequireComponent();
};
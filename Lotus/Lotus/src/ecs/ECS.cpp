#include "ECS.h"
#include "../log/Lotus_Log.h"
#include <algorithm>

int IComponent::nextId = 0;

int Entity::GetId() const
{
    return id;
}

void Entity::Destroy()
{
    registry->RemoveEntity(*this);
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (entities[i] == entity)
        {
            entities.erase(entities.begin() + i);
            i--;
        }
    }
}

std::vector<Entity> System::GetSystemEntities() const
{
    return entities;
}

const Signature& System::GetComponentSignature() const
{
    return componentSignature;
}

void EntityManager::Update()
{
    // Add the entities that are waiting to be created to the active systems
    for (auto entity : entitiesNeedToBeAdded)
    {
        AddEntityToSystems(entity);
    }
    entitiesNeedToBeAdded.clear();

    // Remove the entities that are waiting to be removed from the active systems
    for (auto entity : entitiesNeedToBeRemoved)
    {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();

        // Make the entity id to become available
        freeIds.push_back(entity.GetId());
    }
    entitiesNeedToBeRemoved.clear();
}

Entity EntityManager::CreateEntity()
{
    int entityId;
    // Generate entity ID

    if (freeIds.empty())
    {
        // If there are no free ID to be reused
        entityId = numberOfEntities++;
        // Make sure the entityComponentSignature vector can accomodate the new entity
        if (entityId >= entityComponentSignatures.size())
        {
            entityComponentSignatures.resize(entityId + 1);
        }
    }
    else
    {
        // If there are free ID to be reused, then use it
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    // Create a new entity
    Entity newEntity(entityId);
    newEntity.registry = this;
    // Insert the new entity to the set
    entitiesNeedToBeAdded.insert(newEntity);

    Lotus_Log::Info("Entity created with id: " + std::to_string(entityId));
    return newEntity;
}

void EntityManager::RemoveEntity(Entity entity)
{
    entitiesNeedToBeRemoved.insert(entity);
}

void EntityManager::AddEntityToSystems(Entity entity)
{
    const int entityId = entity.GetId();

    //Match entityComponentSignature and systemComponentSignature
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    // Loop all the systems
    for (auto& system : systems)
    {
        const auto& systemComponentSignature = system.second->GetComponentSignature();

        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
        if (isInterested)
        {
            //Add the entity to the system
            system.second->AddEntityToSystem(entity);
        }
    }
}

void EntityManager::RemoveEntityFromSystems(Entity entity)
{
    for (auto system : systems)
    {
        system.second->RemoveEntityFromSystem(entity);
    }
}

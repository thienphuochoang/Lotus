#include "EntityManager.h"
#include "../log/Lotus_Log.h"
#include "../components/Component.h"

void EntityManager::Update()
{
    // TODO: 
    // Add the entities that are waiting to be created to the active systems
    // Remove the entities that are waiting to be removed from the active systems
}

Entity EntityManager::CreateEntity()
{
    int entityId;
    // Generate entity ID
    entityId = numberOfEntities++;
    // Create a new entity
    Entity newEntity(entityId);
    // Insert the new entity to the set
    entitiesNeedToBeAdded.insert(newEntity);
    Lotus_Log::Info("Entity created with id: " + std::to_string(entityId));
    return newEntity;
}

template<typename TComponent, typename ...TArgs>
void EntityManager::AddComponent(Entity entity, TArgs ...args)
{

}
#include "EntityManager.h"
#include "../log/Lotus_Log.h"
#include "../components/Component.h"

int IComponent::nextId = 0;

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
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    if (componentId >= componentCollections.size())
    {
        componentCollections.resize(componentId + 1, nullptr);
    }

    if (!componentCollections[componentId])
    {
        Collection<TComponent>* newComponentCollection = new Collection<TComponent>();
        componentCollections[componentId] = newComponentCollection;
    }

    Collection<TComponent>* componentCollection = componentCollection[componentId];

    if (entityId >= componentCollection->GetSize())
    {
        componentCollection->Resize(numberOfEntities);
    }
    TComponent newComponent(std::forward<TArgs>(args)...);
    componentCollection->Set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);
}
template <typename TComponent>
void EntityManager::RemoveComponent(Entity entity)
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool EntityManager::HasComponent(Entity entity) const
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    entityComponentSignatures[entityId].test(componentId);
}
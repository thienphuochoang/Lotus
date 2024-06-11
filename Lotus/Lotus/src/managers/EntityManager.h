#pragma once
#include "../collection/Collection.h"
#include "../ecs/ECS.h"
#include "../systems/System.h"
#include "../log/Lotus_Log.h"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include "../log/Lotus_Log.h"
#include "../components/Component.h"
// The entity manager manages the creation and destruction of entities, 
// add systems and components
class EntityManager
{
private:
    int numberOfEntities = 0;
    // Vector of component collections, each collection contains all the data
    // for a certain component type
    // Vector index = component type id
    // Collection index = entity id
    std::vector<std::shared_ptr<ICollection>> componentCollections;

    // Vector of component signatures per entity, saying which component is "turned on"
    // for a given entity
    // Vector index = entity id
    std::vector<Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    // Set of entities that are flagged to be added in the next Update() frame
    std::set<Entity> entitiesNeedToBeAdded;
    // Set of entities that are flagged to be removed in the next Update() frame
    std::set<Entity> entitiesNeedToBeRemoved;
public:
    EntityManager() { Lotus_Log::Info("Registry constructor called! "); };
    ~EntityManager() { Lotus_Log::Info("Registry destructor called! "); };
    void Update();
    // TODO
    // Create an entity
    Entity CreateEntity();
    // Remove an entity
    // AddComponent(Entity entity)
    template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
    // RemoveComponent(Entity entity)
    template <typename TComponent> void RemoveComponent(Entity entity);
    // HasComponent(Entity entity)
    template <typename TComponent> bool HasComponent(Entity entity) const;
    // GetComponent(Entity entity)
    template <typename TComponent> TComponent& GetComponent(Entity entity) const;
    // AddSystem
    template <typename TSystem, typename ...TArgs> void AddSystem(TArgs ...args);
    // RemoveSystem
    template <typename TSystem> void RemoveSystem();
    // HasSystem
    template <typename TSystem> bool HasSystem() const;
    // GetSystem
    template <typename TSystem> TSystem& GetSystem() const;

    // Check the component signature of an entity and add the entity to the systems
    // that are interested in it
    void AddEntityToSystems(Entity entity);
};
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

template<typename TComponent>
TComponent& EntityManager::GetComponent(Entity entity) const
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    auto componentCollection = std::static_pointer_cast<Collection<TComponent>>(componentCollections[componentId]);
    return componentCollection->Get(entityId);
}

template<typename TSystem, typename ...TArgs>
void EntityManager::AddSystem(TArgs ...args)
{
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template<typename TSystem>
void EntityManager::RemoveSystem()
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template<typename TSystem>
bool EntityManager::HasSystem() const
{
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem& EntityManager::GetSystem() const
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *dynamic_cast<TSystem>(system->second);
}

template<typename TComponent, typename ...TArgs>
void EntityManager::AddComponent(Entity entity, TArgs&& ...args)
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    if (componentId >= componentCollections.size())
    {
        componentCollections.resize(componentId + 1, nullptr);
    }

    if (!componentCollections[componentId])
    {
        std::shared_ptr<Collection<TComponent>> newComponentCollection = std::make_shared<Collection<TComponent>>();
        componentCollections[componentId] = newComponentCollection;
    }

    std::shared_ptr<Collection<TComponent>> componentCollection = std::static_pointer_cast<Collection<TComponent>>(componentCollections[componentId]);

    if (entityId >= componentCollection->GetSize())
    {
        componentCollection->Resize(numberOfEntities);
    }
    TComponent newComponent(std::forward<TArgs>(args)...);
    componentCollection->Set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);

    Lotus_Log::Info("Component ID: " + std::to_string(componentId) + " was added to entity ID" + std::to_string(entityId));
}

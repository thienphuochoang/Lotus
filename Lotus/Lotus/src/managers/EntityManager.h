#pragma once
#include "../collection/Collection.h"
#include "../ecs/ECS.h"
#include "../systems/System.h"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
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
    std::vector<ICollection*> componentCollections;

    // Vector of component signatures per entity, saying which component is "turned on"
    // for a given entity
    // Vector index = entity id
    std::vector<Signature> entityComponentSignatures;

    std::unordered_map<std::type_index, System*> systems;

    // Set of entities that are flagged to be added in the next Update() frame
    std::set<Entity> entitiesNeedToBeAdded;
    // Set of entities that are flagged to be removed in the next Update() frame
    std::set<Entity> entitiesNeedToBeRemoved;
public:
    EntityManager() = default;
    void Update();
    // TODO
    // Create an entity
    Entity CreateEntity();
    // Remove an entity
    // AddComponent(Entity entity)
    template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs ...args);
    // RemoveComponent(Entity entity)
    template <typename TComponent> void RemoveComponent(Entity entity);
    // HasComponent(Entity entity)
    template <typename TComponent> bool HasComponent(Entity entity) const;
    // GetComponent(Entity entity)
    template <typename T> T& GetComponent(Entity entity) const;
    // AddSystem
    // RemoveSystem
    // HasSystem
    // GetSystem
};

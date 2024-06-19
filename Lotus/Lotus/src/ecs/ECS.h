#pragma once
#include "../log/Lotus_Log.h"
#include <vector>
#include <bitset>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <deque>
#include <iostream>
const unsigned int MAX_COMPONENTS = 32;
// Signature
// A bitset (1's and 0's) to keep track of which components an entity has
// helps keep track of which entities a system is interested in
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
protected:
    static int nextId;

};
// Used to assign an unique ID to a component type
template <typename T>
class Component : public IComponent
{
public:
    // Get the unique id of Component<T>
    static int GetId()
    {
        static auto id = nextId++;
        return id;
    }
};

class Entity
{
private:
    int id;
public:
    Entity(int id) : id(id) {};
    Entity(const Entity& other) = default;
    int GetId() const;

    Entity& operator =(const Entity& other) = default;
    bool operator ==(const Entity& other) const { return id == other.id; }
    bool operator !=(const Entity& other) const { return id != other.id; }
    bool operator >(const Entity& other) const { return id > other.id; }
    bool operator <(const Entity& other) const { return id < other.id; }
    template<typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template<typename TComponent> void RemoveComponent();
    template<typename TComponent> bool HasComponent() const;
    template<typename TComponent> TComponent& GetComponent() const;
    class EntityManager* registry;
    void Destroy();

    // Tag and group management functions
    void Tag(const std::string& tag);
    bool HasTag(const std::string& tag) const;
    void Group(const std::string& group);
    bool BelongsToGroup(const std::string& group) const;
};

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

class ICollection
{
public:
    virtual ~ICollection() = default;
    virtual void RemoveEntityFromCollection(int entityId) = 0;
};
template <typename T>
class Collection : public ICollection
{
private:
    // We keep track of the vector of objects and the current number of elements
    std::vector<T> data;
    int size;

    // Helper maps to keep track of entity ids per index, so the vector is always packed
    std::unordered_map<int, int> entityIdToIndex;
    std::unordered_map<int, int> indexToEntityId;
public:
    Collection(int capacity = 100)
    {
        size = 0;
        data.resize(capacity);
    }
    virtual ~Collection() = default;
    bool IsEmpty() const;
    int GetSize() const;
    void Resize(int n);
    void Clear();
    void Add(T object);
    void Set(int index, T object);
    T& Get(int index);
    T& operator [](unsigned int index);
    void Remove(int entityId);
    void RemoveEntityFromCollection(int entityId) override;
};

template <typename T>
bool Collection<T>::IsEmpty() const
{
    return size == 0;
}

template<typename T>
int Collection<T>::GetSize() const
{
    return size;
}

template<typename T>
void Collection<T>::Resize(int n)
{
    data.resize(n);
}

template<typename T>
void Collection<T>::Clear()
{
    data.clear();
    size = 0;
}

template<typename T>
void Collection<T>::Add(T object)
{
    data.push_back(object);
}

template<typename T>
void Collection<T>::Set(int entityId, T object)
{
    if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
    {
        // If the element already exists, simply replace the component object
        int index = entityIdToIndex[entityId];
        data[index] = object;
    }
    else
    {
        // When adding a new object, we keep track of the entity ids
        // and their vector
        int index = size;
        entityIdToIndex.emplace(entityId, index);
        indexToEntityId.emplace(index, entityId);
        if (index >= data.capacity())
        {
            // We resize by always doubling the current capacity
            data.resize(size * 2);
        }
        data[index] = object;
        size++;
    }
}

template<typename T>
T& Collection<T>::Get(int entityId)
{
    int index = entityIdToIndex[entityId];
    return static_cast<T&>(data[index]);
}

template<typename T>
T& Collection<T>::operator [](unsigned int index)
{
    return data[index];
}

template<typename T>
void Collection<T>::Remove(int entityId)
{
    // Copy the last element to the removed position to keep the array packed
    int indexOfRemoved = entityIdToIndex[entityId];
    int indexOfLast = size - 1;
    data[indexOfRemoved] = data[indexOfLast];

    // Update the index-entity maps to point to the correct elements
    int entityIdOfLastElement = indexToEntityId[indexOfLast];
    entityIdToIndex[entityIdOfLastElement] = indexOfRemoved;
    indexToEntityId[indexOfRemoved] = entityIdOfLastElement;

    entityIdToIndex.erase(entityId);
    indexToEntityId.erase(indexOfLast);

    size--;
}

template<typename T>
void Collection<T>::RemoveEntityFromCollection(int entityId)
{
    if (entityIdToIndex.find(entityId) != entityIdToIndex.end())
    {
        Remove(entityId);
    }
}

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

    // List of free entity ids that are removed
    std::deque<int> freeIds;

    // Entity tags (one tag name per entity)
    std::unordered_map<std::string, Entity> entityPerTag;
    std::unordered_map<int, std::string> tagPerEntity;

    // Entity groups (a set of entities per group name)
    std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
    std::unordered_map<int, std::string> groupPerEntity;

public:
    EntityManager() { Lotus_Log::Info("Registry constructor called! "); };
    ~EntityManager() { Lotus_Log::Info("Registry destructor called! "); };
    void Update();
    // Create an entity
    Entity CreateEntity();
    // Remove an entity
    void RemoveEntity(Entity entity);
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

    // Remove entities from the systems
    void RemoveEntityFromSystems(Entity entity);

    // Tag management functions
    void TagEntity(Entity entity, const std::string& tag);
    bool EntityHasTag(Entity entity, const std::string& tag) const;
    Entity GetEntityByTag(const std::string& tag) const;
    void RemoveEntityTag(Entity entity);

    // Group management
    void GroupEntity(Entity entity, const std::string& group);
    bool EntityBelongsToGroup(Entity entity, const std::string& group) const;
    std::vector<Entity> GetEntitiesByGroup(const std::string& group) const;
    void RemoveEntityGroup(Entity entity);
};
template <typename TComponent>
void EntityManager::RemoveComponent(Entity entity)
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    entityComponentSignatures[entityId].set(componentId, false);

    // Remove the component from the component list for that entity
    std::shared_ptr<Collection<TComponent>> componentCollection = std::static_pointer_cast<Collection<TComponent>>(componentCollections[componentId]);
    componentCollection->Remove(entityId);

    // Set this component signature for that entity to false
    entityComponentSignatures[entityId].set(componentId, false);

    Lotus_Log::Info("Component ID: " + std::to_string(componentId) + " was REMOVED from entity ID" + std::to_string(entityId));
}

template <typename TComponent>
bool EntityManager::HasComponent(Entity entity) const
{
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
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
    return *(std::static_pointer_cast<TSystem>(system->second));
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

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentCollection->Set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);

    Lotus_Log::Info("Component ID: " + std::to_string(componentId) + " was added to entity ID" + std::to_string(entityId));
}

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args)
{
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template<typename TComponent>
void Entity::RemoveComponent()
{
    registry->RemoveComponent<TComponent>(*this);
}

template<typename TComponent>
bool Entity::HasComponent() const
{
    return registry->HasComponent<TComponent>(*this);
}

template<typename TComponent>
TComponent& Entity::GetComponent() const
{
    return registry->GetComponent<TComponent>(*this);
}

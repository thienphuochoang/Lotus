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

void Entity::Tag(const std::string& tag)
{
    registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const
{
    return registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string& group)
{
    registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group) const
{
    return registry->EntityBelongsToGroup(*this, group);
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

        // Remove the entity from the component collections
        for (auto collection : componentCollections)
        {
            if (collection)
                collection->RemoveEntityFromCollection(entity.GetId());
        }

        // Make the entity id to become available
        freeIds.push_back(entity.GetId());

        // Remove this entity from groups and tags
        RemoveEntityGroup(entity);
        RemoveEntityTag(entity);
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

void EntityManager::TagEntity(Entity entity, const std::string& tag)
{
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
}

bool EntityManager::EntityHasTag(Entity entity, const std::string& tag) const
{
    if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end())
        return false;
    return entityPerTag.find(tag)->second == entity;
}

Entity EntityManager::GetEntityByTag(const std::string& tag) const
{
    return entityPerTag.at(tag);
}

void EntityManager::RemoveEntityTag(Entity entity)
{
    auto taggedEntity = tagPerEntity.find(entity.GetId());
    if (taggedEntity != tagPerEntity.end())
    {
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
}

void EntityManager::GroupEntity(Entity entity, const std::string& group)
{
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

bool EntityManager::EntityBelongsToGroup(Entity entity, const std::string& group) const
{
    if (entitiesPerGroup.find(group) == entitiesPerGroup.end())
        return false;
    auto groupEntities = entitiesPerGroup.at(group);
    return groupEntities.find(entity.GetId()) != groupEntities.end();
}

std::vector<Entity> EntityManager::GetEntitiesByGroup(const std::string& group) const
{
    auto& setOfEntities = entitiesPerGroup.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void EntityManager::RemoveEntityGroup(Entity entity)
{
    auto groupedEntity = groupPerEntity.find(entity.GetId());
    if (groupedEntity != groupPerEntity.end())
    {
        auto group = entitiesPerGroup.find(groupedEntity->second);
        if (group != entitiesPerGroup.end())
        {
            auto entityInGroup = group->second.find(entity);
            if (entityInGroup != group->second.end())
            {
                group->second.erase(entityInGroup);
            }
        }
        groupPerEntity.erase(groupedEntity);
    }
}

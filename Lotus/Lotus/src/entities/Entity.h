#pragma once
class Entity
{
private:
    int id;
public:
    Entity(int id) : id(id) {};
    Entity(const Entity& other) = default;
    int GetId() const;

    Entity& operator =(const Entity& other) = default;
    bool operator ==(const Entity& other) const;
    bool operator !=(const Entity& other) const;
    bool operator <(const Entity& other) const;
    template<typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
    template<typename TComponent> void RemoveComponent();
    template<typename TComponent> bool HasComponent() const;
    template<typename TComponent> TComponent& GetComponent() const;
    class EntityManager* registry;
};

template<typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs && ...args)
{
    // TODO: insert return statement here
}

template<typename TComponent>
void Entity::RemoveComponent()
{
    // TODO: insert return statement here
}

template<typename TComponent>
bool Entity::HasComponent() const
{
    return false;
}

template<typename TComponent>
inline TComponent& Entity::GetComponent() const
{
    // TODO: insert return statement here
}
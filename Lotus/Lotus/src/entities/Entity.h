#pragma once
class Entity
{
private:
    int id;
public:
    Entity(int inputId) { id = inputId; }
    Entity(const Entity& other) = default;
    int GetId() const;

    Entity& operator =(const Entity& other) = default;
    bool operator ==(const Entity& other) const;
    bool operator <(const Entity& other) const { return id < other.id; }
};
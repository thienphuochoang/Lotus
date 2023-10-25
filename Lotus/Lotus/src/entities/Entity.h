#pragma once
class Entity
{
private:
    int id;
public:
    Entity(int inputId) { id = inputId; }
    int GetId() const;
};
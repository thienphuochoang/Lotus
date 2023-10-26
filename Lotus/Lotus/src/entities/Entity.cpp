#include "Entity.h"
int Entity::GetId() const
{
    return id;
}
bool Entity::operator ==(const Entity& other) const
{
    return id == other.id;
}
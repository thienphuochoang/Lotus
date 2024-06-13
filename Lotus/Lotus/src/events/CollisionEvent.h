#pragma once
#include "../ecs/ECS.h"
class CollisionEvent : public Event
{
public:
    Entity a;
    Entity b;
    CollisionEvent(Entity a, Entity b) : a(a), b(b)
    {

    }
};
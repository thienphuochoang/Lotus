#pragma once
class MovementSystem : public System
{
public:
    MovementSystem()
    {
        // TODO: 
        // RequireComponent <TransformComponent>();
        // RequireComponent <...>();
    }
    void Update()
    {
        // TODO: Loop all entities
        for (auto object : GetEntities())
        {
            // TODO:
            // Update entity position based on its velocity every frame of the loop
        }
    }

};
#pragma once
#include "System.h"
#include "../managers/EntityManager.h"
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
        for (auto object : EntityManager::GetEntities())
        {
            // TODO:
            // Update entity position based on its velocity every frame of the loop
        }
    }

};
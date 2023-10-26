#include "System.h"
#include "../components/Component.h"
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

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}
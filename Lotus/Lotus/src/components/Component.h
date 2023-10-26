#pragma once
struct IComponent
{
protected:
    static int nextId;

};
// Used to assign an unique ID to a component type
template <typename T>
class Component : public IComponent
{
    // Get the unique id of Component<T>
    static int GetId()
    {
        static auto id = nextId++;
        return id;
    }
};
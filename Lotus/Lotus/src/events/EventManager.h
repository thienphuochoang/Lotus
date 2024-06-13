#pragma once
#include "../log/Lotus_Log.h"
#include <map>
#include <typeindex>
#include <memory>
#include <list>

class Event
{
public:
    Event() = default;
};

class IEventCallback
{
public:
    virtual ~IEventCallback() = default;
    void Execute(Event& e)
    {
        Call(e);
    }
private:
    virtual void Call(Event& e) = 0;
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback
{
private:
    typedef void (TOwner::* CallbackFunction)(TEvent&);

    TOwner* owner;
    CallbackFunction callbackFunction;
    virtual void Call(Event& e) override
    {
        std::invoke(callbackFunction, owner, static_cast<TEvent&>(e));
    }
public:
    EventCallback(TOwner* owner, CallbackFunction callbackFunction) : 
        owner(owner), callbackFunction(callbackFunction)
    {}
    virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;
class EventManager
{
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
public:
    EventManager()
    {
        Lotus_Log::Info("EventManager constructor called");
    }
    ~EventManager()
    {
        Lotus_Log::Info("EventManager destructor called");
    }

    // Subscribe to an event type <T>
    // A listener subscribes to an event
    // Example: eventManager->SubscribeToEvent<CollisionEvent>(&Lotus_SDL::onCollision);
    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* owner, void (TOwner::*callbackFunction)(TEvent&))
    {
        if (!subscribers[typeid(TEvent)].get())
        {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(owner, callbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    // Emit an event of type <T>
    // As soon as something emits an event, execute all the listener callback functions
    // Example: eventManager->EmitEvent<CollisionEvent>(player, enemy);
    template <typename TEvent, typename ...TArgs>
    void EmitEvent(TArgs&& ...args)
    {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers)
        {
            for (auto i = handlers->begin(); i != handlers->end(); i++)
            {
                auto handler = i->get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }

    // Clear the subscribers list
    void Reset()
    {
        subscribers.clear();
    }
};
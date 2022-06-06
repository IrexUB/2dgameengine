#pragma once

#include <map>
#include <functional>
#include <vector>
#include <memory>
#include <typeindex>
#include "../Logger/Logger.h"
#include "Event.h"

class IEventCallback
{
public:
    void Execute(Event& event)
    {
        Call(event);
    }
    virtual ~IEventCallback() = default;
private:
    virtual void Call(Event& event) = 0;
};


template <typename EventType>
class EventCallback : public IEventCallback
{
public:
    using Callback = std::function<void(EventType&)>;

    EventCallback(Callback callback)
        : m_callback(std::move(callback))
    {}
private:
    virtual void Call(Event& event) override
    {
        if (m_callback) {
            m_callback(static_cast<EventType&>(event));
        }
    }
    Callback m_callback;
};

class EventBus
{
    using HandlerList = std::vector<std::unique_ptr<IEventCallback>>;

    public:
        template <typename EventType, typename... EventArgs>
        void EmitEvent(EventArgs&&... args) {
            auto& handlers = m_subscribers[typeid(EventType)];
            if (!handlers.empty()) {
                EventType event(std::forward<EventArgs>(args)...);
                for (auto& handler : handlers)
                    handler->Execute(event);
            }
        }

        template <typename EventType>
        void SubscribeToEvent(std::function<void (EventType&)> callback)
        {
            auto _callback = std::make_unique<EventCallback<EventType>>(callback);
            m_subscribers[typeid(EventType)].push_back(std::move(_callback));
        }

    private:    
        std::map<std::type_index, HandlerList> m_subscribers;
};


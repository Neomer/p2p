#include "EventBus.h"
#include <QDebug>

EventBus::EventBus()
{
    
}

void EventBus::publish(QString event, QVariant data)
{
    if (!_hashMap.contains(event))
        return;
    foreach (IEventDispatcher *d, _hashMap[event]) 
    {
        d->onEventCatch(this, event, data);
    }
}

void EventBus::subscribe(QString event, IEventDispatcher *object)
{
    _hashMap[event] << object;
}

void EventBus::unsubscribe(QString event, IEventDispatcher *object)
{
    if (_hashMap.contains(event))
    {
        if (_hashMap["event"].contains(object))
        {
            if (!_hashMap["event"].removeOne(object))
            {
                qDebug() << "Removing dispatcher failed!";
                throw std::runtime_error("Removing dispatcher failed!");
            }
        }
    }
}

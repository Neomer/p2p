#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "IEventDispatcher.h"
#include <QHash>

class EventBus
{
public:
    EventBus();
    
    void publish(QString event, QVariant data);
    void subscribe(QString event, IEventDispatcher *object);
    void unsubscribe(QString event, IEventDispatcher *object);
    
private:
    QHash<QString, QList<IEventDispatcher *>> _hashMap;
};

#endif // EVENTBUS_H

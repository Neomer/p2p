#include "Peer.h"

Peer::Peer(QTcpSocket *socket, QObject *parent) :
    QObject(parent),
    _socket(socket)
{
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(_socket, SIGNAL(disconnected()), this, SIGNAL(connectionLost()));
}

Peer::PeerPackage Peer::readPackage()
{
    PeerPackage ret;
    if ((uint)_buffer.size() < sizeof(PeerPackage))
        return ret;

    memcpy(&ret, _buffer.constData(), sizeof(PeerPackage));
    _buffer.remove(0, sizeof(PeerPackage));
    return ret;
}

void Peer::readData()
{
    qDebug() << "From:" << _socket->peerAddress().toString() << "Received" <<_socket->bytesAvailable() << "byte(s)";
    _buffer.append(_socket->readAll());
    if ((uint)_buffer.size() >= sizeof(PeerPackage))
        emit packageReady();
}

#ifndef PEER_H
#define PEER_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>
#define PACKAGE_NODE                        0x00
#define PACKAGE_NODE_REGISTER_PEER          PACKAGE_NODE + 1

#define PACKAGE_PEER                        0x20
#define PACKAGE_PEER_DATA                   PACKAGE_PEER + 1

class Peer : public QObject
{
    Q_OBJECT


public:
    struct PeerPackage
    {
        int Type;
        char Data[128];
    };

    explicit Peer(QTcpSocket *socket, QObject *parent = 0);
    PeerPackage readPackage();

private slots:
    void readData();

signals:
    void packageReady();
    void connectionLost();

private:
    QTcpSocket *_socket;
    QByteArray _buffer;
};

#endif // PEER_H

#ifndef PEERCOLLECTION_H
#define PEERCOLLECTION_H

#include <QObject>
#include <QList>
#include "Peer.h"

class PeerCollection : public QObject
{
    Q_OBJECT
public:
    explicit PeerCollection(QObject *parent = nullptr);

    void addPeer(Peer *value);

private slots:
    void peerDisconnected();

private:
    QList<Peer *> _peers;
};

#endif // PEERCOLLECTION_H

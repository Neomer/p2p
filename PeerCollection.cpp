#include "PeerCollection.h"

PeerCollection::PeerCollection(QObject *parent) : QObject(parent)
{
}

void PeerCollection::addPeer(Peer *value)
{
    connect(value, SIGNAL(connectionLost()), this, SLOT(peerDisconnected()));
    _peers.append(value);
}

void PeerCollection::peerDisconnected()
{
    _peers.removeOne((Peer *)sender());
}

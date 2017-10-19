#ifndef BLOCKCHAININDEX_H
#define BLOCKCHAININDEX_H

#include <QObject>

#include "core/Hash.h"

class BlockChainIndex
{
public:
    BlockChainIndex();
    
    quint64 find(Hash hash);
};

#endif // BLOCKCHAININDEX_H

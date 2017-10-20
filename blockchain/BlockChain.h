#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <QStringList>
#include <core/Hash.h>
#include <core/Context.h>
#include <blockchain/BlockChainIndex.h>
#include <blockchain/Block.h>

class BlockChain
{
public:
    BlockChain();
    
    void load();
    bool find(Hash h, Block *b);
    bool save(Block *b);
    bool contains(Hash h);
    
    
private:
    QStringList getPathFromHash(Hash h);
};

#endif // BLOCKCHAIN_H

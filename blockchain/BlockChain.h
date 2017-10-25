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
    static BlockChain &instance()
    {
        static BlockChain i;
        return i;
    }
    
    void load();
    bool find(Block *b, Hash h);
    bool save(Block *b);
    bool contains(Hash h);
    
    
private:
    BlockChain();
    BlockChain(const BlockChain& other);
    BlockChain &operator =(const BlockChain& other);
    
    QStringList getPathFromHash(Hash h);
};

#endif // BLOCKCHAIN_H

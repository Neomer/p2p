#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <QStringList>
#include <core/Hash.h>
#include <core/Context.h>
#include <blockchain/BlockChainIndex.h>
#include <blockchain/Block.h>
#include <core/ISerializable.h>

class BlockChain: public ISerializable
{
    Q_OBJECT
    
    Q_PROPERTY(quint64 lastBlockNumber READ getLastBlockNumber WRITE setLastBlockNumber)
    
public:
    static BlockChain &instance()
    {
        static BlockChain i;
        return i;
    }
    
    void load();
    
    bool find(Block *b, Hash h);
    bool appendBlock(Block *b);
    bool contains(Hash h);
    
    
    quint64 getLastBlockNumber() const
    {
        return m_lastBlockNumber;
    }
    
public slots:
    void setLastBlockNumber(quint64 lastBlockNumber)
    {
        m_lastBlockNumber = lastBlockNumber;
    }
    
private:
    BlockChain();
    BlockChain(const BlockChain& other);
    BlockChain &operator =(const BlockChain& other);
    
    QStringList getPathFromHash(Hash h);
    quint64 m_lastBlockNumber;
};

#endif // BLOCKCHAIN_H

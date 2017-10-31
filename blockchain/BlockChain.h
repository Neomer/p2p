#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <QStringList>
#include <core/Hash.h>
#include <core/Context.h>
#include <blockchain/BlockChainIndex.h>
#include <blockchain/Block.h>
#include <core/ISerializable.h>
#include <core/IEventDispatcher.h>

class BlockChain: public ISerializable, public IEventDispatcher
{
    Q_OBJECT
    
    Q_PROPERTY(quint64 lastBlockNumber READ getLastBlockNumber WRITE setLastBlockNumber)
    Q_PROPERTY(QString lastBlockHash READ getLastBlockHash WRITE setLastBlockHash)
	Q_PROPERTY(QString difficulty READ getDifficulty WRITE setDifficulty)
    
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
    
    QString getLastBlockHash() const
    {
        return m_lastBlockHash;
    }

	QString getDifficulty() const
	{
		return m_difficulty;
	}

	bool onEventCatch(void *bus, QString event, QVariant data);
	
public slots:
    void setLastBlockNumber(quint64 lastBlockNumber)
    {
        m_lastBlockNumber = lastBlockNumber;
    }
    
    void setLastBlockHash(QString lastBlockHash)
    {
        m_lastBlockHash = lastBlockHash;
    }
	
	void setDifficulty(QString difficulty)
	{
		m_difficulty = difficulty;
	}
	
private:
    BlockChain();
    BlockChain(const BlockChain& other);
    BlockChain &operator =(const BlockChain& other);
	
    QStringList getPathFromHash(Hash h);
	void updateDifficulty();
	
    quint64 m_lastBlockNumber;
    QString m_lastBlockHash;
	QString m_difficulty;
};

#endif // BLOCKCHAIN_H

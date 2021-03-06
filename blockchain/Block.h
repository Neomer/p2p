#ifndef BLOCK_H
#define BLOCK_H

#include <QDateTime>

#include <core/ISerializable.h>
#include <core/Hash.h>

class Block : public ISerializable
{
    Q_OBJECT
    
    Q_PROPERTY(quint32 number READ getNumber WRITE setNumber)
    Q_PROPERTY(QString data READ getData WRITE setData)
    Q_PROPERTY(QString previousBlock READ getPreviousBlock WRITE setPreviousBlock)
    Q_PROPERTY(QDateTime creationTime READ getCreationTime WRITE setCreationTime)
    Q_PROPERTY(int nonce READ getNonce WRITE setNonce)
    Q_PROPERTY(bool confirmed READ getConfirmed WRITE setConfirmed STORED false)
    Q_PROPERTY(int version READ getVersion WRITE setVersion)
    
public:
    Block(QObject *parent = 0);
	Block &operator =(const Block &other);
    
    quint32 getNumber() const
    {
        return m_number;
    }
    
    QString getData() const
    {
        return m_data;
    }
    
    QString getPreviousBlock() const
    {
        return m_previousBlock;
    }
    
    
    QDateTime getCreationTime() const
    {
        return m_creationTime;
    }
    
    int getNonce() const
    {
        return m_nonce;
    }
    
    bool getConfirmed() const
    {
        return m_confirmed;
    }
    
    int getVersion() const
    {
        return m_version;
    }
    
public slots:
    void setNumber(quint32 number)
    {
        m_number = number;
    }
    
    void setData(QString data)
    {
        m_data = data;
    }
    
    void setPreviousBlock(QString previousBlock)
    {
        m_previousBlock = previousBlock;
    }

    void setPreviousBlock(Hash hash)
    {
        m_previousBlock = hash.toString();
    }
    
    void setCreationTime(QDateTime creationTime)
    {
        m_creationTime = creationTime;
    }
    
    void setNonce(int nonce)
    {
        m_nonce = nonce;
    }
    
    void setConfirmed(bool confirmed)
    {
        m_confirmed = confirmed;
    }
    
    void setVersion(int version)
    {
        m_version = version;
    }
    
private:
    quint32 m_number;
    QString m_data;
    QString m_previousBlock;
    QDateTime m_creationTime;
    int m_nonce;
    bool m_confirmed;
    int m_version;
};

#endif // BLOCK_H

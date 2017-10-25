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
    Q_PROPERTY(QDateTime creationTime READ creationTime WRITE setCreationTime)
    
public:
    Block(QObject *parent = 0);
    
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
    
    
    QDateTime creationTime() const
    {
        return m_creationTime;
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
    
private:
    quint32 m_number;
    QString m_data;
    QString m_previousBlock;
    QDateTime m_creationTime;
};

#endif // BLOCK_H

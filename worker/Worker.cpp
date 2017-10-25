#include "Worker.h"
#include <blockchain/BlockChain.h>

#define MAX_NONCE       (quint64)1844674407370951615

Worker::Worker(QObject *parent) :
    QThread(parent)
{
    _tmrHashrate = new QTimer(this);
    connect(_tmrHashrate, SIGNAL(timeout()), this, SLOT(tick()));
    _tmrHashrate->setInterval(10000);
    _tmrHashrate->start();
    
    start();
}

void Worker::addBlock(Block *b)
{
    _blocks << b;
}

void Worker::tick()
{
    qint64 ss = QDateTime::currentSecsSinceEpoch();
    if (_nonce - _nonceSave > 0)
    {
        qDebug() << "Hashrate:" << (_nonce - _nonceSave) / (ss - _tt) << "h/s";
    }
    
    _nonceSave = _nonce;
    _tt = ss;
}

void Worker::run()
{
    while (true)
    {
        if (!_blocks.isEmpty())
        {
            _tt = QDateTime::currentSecsSinceEpoch();
            Block *b = _blocks.takeFirst();
            qDebug() << "Block" << b->getNumber() << "began to be mined...";
            while (true)
            {
                for (_nonceSave = _nonce = 0; _nonce < MAX_NONCE; _nonce++)
                {
                    b->setNonce(_nonce);
                    QJsonObject o = b->serialize();
                    if (Hash(o["hash"].toString()) < Hash("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"))
                    {
                        //emit blockGenerated(b);
                        BlockChain::instance().save(b);
                        break;
                    }
                }
                if (_nonce == MAX_NONCE)
                {
                    qDebug() << "Block confirmation failed! Try with new date...";
                    b->setCreationTime(QDateTime::currentDateTime());
                }
                else
                {
                    qDebug() << "Block confirmed!";
                    break;
                }
            }
        }
        msleep(1000);
    }
}

#include "Worker.h"
#include <blockchain/BlockChain.h>

#define MAX_NONCE       (quint64)1844674407370951615

Worker::Worker(QObject *parent) :
    QThread(parent),
    _stop(false)
{
    //_tmrHashrate = new QTimer(this);
    //connect(_tmrHashrate, SIGNAL(timeout()), this, SLOT(tick()));
    //_tmrHashrate->setInterval(60000);
    //_tmrHashrate->start();
    
    start();
}

void Worker::addBlock(Block *b)
{
    _stop = false;
    _blocks << b;
}

void Worker::stopWork()
{
    _stop = true;
}

double Worker::hps()
{
    qint64 ss = QDateTime::currentSecsSinceEpoch();
    return (_nonce - _nonceSave) / (ss - _tt);
    _nonceSave = _nonce;
    _tt = ss;
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
            _stopped = false;
            qDebug() << "Block" << b->getNumber() << "began to be mined...";
            while (true)
            {
                QJsonObject jo = b->serialize(false);
                for (_nonceSave = _nonce = b->getNonce(); _nonce < MAX_NONCE; _nonce++)
                {
                    if (_stop)
                    {
                        qDebug() << "Work terminated!";
                        break;
                    }
                    
                    jo["nonce"] = ISerializable::toJsonValue(_nonce);
                    if (Hash::hash(jo) < Hash("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"))
                    {
                        //emit blockGenerated(b);
                        b->setNonce(_nonce);
                        b->setConfirmed(true);
                        emit blockGenerated(b);
                        break;
                    }
                }
                if (_stop)
                {
                    _stopped = true;
                    break;
                }
                if (_nonce == MAX_NONCE)
                {
                    qDebug() << "Block confirmation failed! Try with new date...";
                    b->setCreationTime(QDateTime::currentDateTime());
                }
                else
                {
                    _stopped = true;
                    qDebug() << "Block confirmed!";
                    break;
                }
            }
        }
        msleep(1000);
    }
}

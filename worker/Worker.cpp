#include "Worker.h"
#include <blockchain/BlockChain.h>

#define MAX_NONCE       (quint64)1844674407370951615

Worker::Worker(QObject *parent) :
    QThread(parent),
    _stop(false),
	_currentWork(0)
{
    start();
}


void Worker::stopWork()
{
	_stop = true;
}

void Worker::createWork(Work *work)
{
	_currentWork = work;
}

double Worker::hps()
{
    qint64 ss = QDateTime::currentMSecsSinceEpoch() * 0.001;
    return (_nonce - _nonceSave) / (ss - _tt);
    _nonceSave = _nonce;
    _tt = ss;
}

void Worker::tick()
{
    qint64 ss = QDateTime::currentMSecsSinceEpoch() * 0.001;
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
        if (_currentWork)
        {
            _tt = QDateTime::currentMSecsSinceEpoch() * 0.001;
            Block *b = _currentWork->block();
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
                    if (Hash::hash(jo) < _currentWork->goal())
                    {
                        b->setNonce(_nonce);
                        b->setConfirmed(true);
						emit workComplete(_currentWork, this);
						_currentWork = 0;
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

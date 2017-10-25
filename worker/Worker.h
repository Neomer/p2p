#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include <blockchain/Block.h>

class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    
    void addBlock(Block *b);
    
signals:
    void blockGenerated(Block *);
    
private slots:
    void tick();
    
protected:
    void run();
    
private:
    QList<Block *> _blocks;
    QTimer *_tmrHashrate;
    qint64 _tt;
    quint64 _nonce, _nonceSave;
};

#endif // WORKER_H

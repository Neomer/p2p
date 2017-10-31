#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include <blockchain/Block.h>
#include <worker/Work.h>

class Worker : public QThread
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    
    void addBlock(Block *b);
    void stopWork();
	void createWork(Work work);
    
    bool isStopped() { return _stopped; }
    
    double hps();
    
signals:
    void blockGenerated(Block *);
    
private slots:
    void tick();
    
protected:
    void run();
    
private:
    QList<Work> _blocks;
    QTimer *_tmrHashrate;
    qint64 _tt;
    quint64 _nonce, _nonceSave;
    bool _stop, _stopped;
};

#endif // WORKER_H

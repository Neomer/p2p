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
    
    void stopWork();
	void createWork(Work *work);
	Work* currentWork() { return _currentWork; }	
    
    bool isStopped() { return _stopped; }
    
    double hps();
    
signals:
	void workComplete(Work *, Worker*);
    
private slots:
    void tick();
    
protected:
    void run();
    
private:
    //QList<Work> _works;
	Work *_currentWork;
    QTimer *_tmrHashrate;
    qint64 _tt;
    quint64 _nonce, _nonceSave;
    bool _stop, _stopped;
};

#endif // WORKER_H

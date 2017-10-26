#ifndef MINER_H
#define MINER_H

#include <QObject>
#include <QList>
#include <QTimer>

#include <worker/Worker.h>

class Miner : public QThread
{
    Q_OBJECT
    
public:
    explicit Miner(int threads, QObject *parent = nullptr);
    void startMine();
    
private slots:
    void blockFound(Block *b);
    void tickHPS();
    
signals:
    void blockMined(Block *);
    
private:
    QList<Worker *> _workers;
    QTimer _tmr;
    
    // QThread interface
protected:
    void run();
};

#endif // MINER_H

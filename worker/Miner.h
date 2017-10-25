#ifndef MINER_H
#define MINER_H

#include <QObject>

#include <worker/Worker.h>

class Miner : public QThread
{
    Q_OBJECT
    
public:
    explicit Miner(QObject *parent = nullptr);
    
};

#endif // MINER_H

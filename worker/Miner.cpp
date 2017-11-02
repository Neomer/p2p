#include "Miner.h"
#include <blockchain/BlockChain.h>

Miner::Miner(int threads, QObject *parent) : 
    QThread(parent)
{
    _tmr.setInterval(60000);
    connect(&_tmr, SIGNAL(timeout()), this, SLOT(tickHPS()));
    _tmr.start();

    for (int i = 0; i < threads; i++)
    {
        Worker *w = new Worker(this);
        //connect(w, SIGNAL(blockGenerated(Block*)), this, SLOT(blockFound(Block*)));
		connect(w, SIGNAL(workComplete(Work*, Worker*)), this, SLOT(workComplete(Work*, Worker*)));
        _workers << w;
    }
}

void Miner::startMine()
{
    Block *nb = new Block();
    nb->setCreationTime(QDateTime::currentDateTime());
    nb->setNumber(BlockChain::instance().getLastBlockNumber() + 1);
    nb->setPreviousBlock(BlockChain::instance().getLastBlockHash());
    foreach (Worker *w, _workers) 
    {
        w->createWork(new Work(nb, BlockChain::instance().getDifficulty()));
    }
}

void Miner::blockFound(Block *b)
{
    foreach (Worker *w, _workers) 
    {
        w->stopWork();
        while (!w->isStopped());
    }
    Context::instance().busMain.publish("block.new", QVariant::fromValue((qint64)b));
    emit blockMined(b);
    startMine();
}

void Miner::tickHPS()
{
    double res = 0;
    foreach (Worker *w, _workers) 
    {
        res += w->hps();
    }
    if (res > 0)
    {
        qDebug() << "Hashrate:" << res << "hps";
	}
}

void Miner::workComplete(Work *work, Worker *worker)
{
	foreach (Worker *w, _workers) 
    {
		if (w != worker)
		{
			w->stopWork();
			while (!w->isStopped());
		}
	}
    //Context::instance().busMain.publish("block.new", QVariant::fromValue((qint64)work->block()));
    emit blockMined(work->block());
    startMine();
}

void Miner::run()
{
    
}

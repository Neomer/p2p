#ifndef PIPECONTROLLER_H
#define PIPECONTROLLER_H

#include <QThread>
#include "Gateway.h"
#include <core/KeyCommand.h>
#include <core/IEventDispatcher.h>
#include <blockchain/Block.h>

class PipeController : public QThread, public IEventDispatcher
{
    Q_OBJECT

public:
    PipeController(Gateway *gateway, QObject * parent = 0);

    void terminate();
    void send(PipePackage data);

public slots:
    void addPipe(Pipe *pipe);
    
// Key Commands
    void getCount(KeyCommand command);
    void askShares(KeyCommand command);
    void createPipe(KeyCommand command);

protected:
    void run();

signals:
    void pipeCommand(QString);

private slots:
    void pipePackage(PipePackage);

private:
	void sendBlock(Block *block);
	
    QList<Pipe *> _pipeList;
	
	// IEventDispatcher interface
public:
	bool onEventCatch(void *bus, QString event, QVariant data) override;
};

#endif // PIPECONTROLLER_H

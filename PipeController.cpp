#include "PipeController.h"
#include "core/CommandProvider.h"
#include "Global.h"
#include <core/Context.h>
#include <blockchain/BlockChain.h>

PipeController::PipeController(Gateway *gateway, QObject *parent) :
    QThread(parent)
{
    connect(gateway, SIGNAL(pipeOpened(Pipe*)), this, SLOT(addPipe(Pipe*)));
    CommandProvider::instance().registerCommand(this, "pipe");
	Context::instance().busMain.subscribe("block.new", this);
	
	
    start(Priority::LowPriority);
}

void PipeController::terminate()
{
    qDebug() << "PipeController() - Thread stopped.";
    QThread::terminate();
}

void PipeController::send(PipePackage data)
{
    foreach (Pipe *p, _pipeList)
    {
        p->send(data);
    }
}

void PipeController::addPipe(Pipe *pipe)
{
    qDebug() << "New pipe" << (qint64)pipe;
    _pipeList << pipe;
    connect(pipe, &Pipe::packageReceived, this, &PipeController::packageReceived);
}

void PipeController::getCount(KeyCommand command)
{
    Q_UNUSED(command);
    qDebug() << "Pipes:" << _pipeList.count();
}

void PipeController::askShares(KeyCommand command)
{
    Q_UNUSED(command);
    send(PipePackage(PACKAGE_COMMAND_SHARE_REQUEST));
}

void PipeController::createPipe(KeyCommand command)
{
    addPipe(new Pipe(command.args().at(0)));
}


void PipeController::run()
{
    qDebug() << "PipeController() - Thread started...";

    forever
    {
        foreach (Pipe *p, _pipeList)
        {
            if (p->connectionsFails() > 5)
            {
                qDebug() << "Connection fail limit exceeded! Closing pipe.";
                _pipeList.removeOne(p);
                p->deleteLater();
            }
        }
        msleep(1000);
    }
}

void PipeController::packageReceived(std::shared_ptr<PipePackage> pkg)
{
    switch (pkg->command())
    {
        case PACKAGE_COMMAND_NEW_BLOCK:
            qDebug() << "New block received!";
            std::shared_ptr<Block> b(new Block());
            QJsonParseError err;
            QJsonDocument json = QJsonDocument::fromJson(pkg->data(), &err);
            if (err.error != QJsonParseError::NoError)
            {
                qDebug() << "Json parsing error!";
                return;
            }
            if (!b->deserialize(json.object()))
            {
                qDebug() << "Block is corrupted!";
                return;
            }
            emit pipeBlock(b);
            break;
    }
}



void PipeController::sendBlock(Block *block)
{
	send(PipePackage(PACKAGE_COMMAND_NEW_BLOCK, ISerializable::prepareForSocket(block->serialize())));
}

bool PipeController::onEventCatch(void *bus, QString event, QVariant data)
{
	if (bus == &(Context::instance().busNetwork))
    {
    }
    else if (bus == &(Context::instance().busMain))
    {
        if (event == "block.new")
        {
            Block *b = (Block *)data.toInt();
			this->sendBlock(b);
			return true;
        }
    }
	return false;
}

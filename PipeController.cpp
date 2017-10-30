#include "PipeController.h"
#include "core/CommandProvider.h"
#include "Global.h"
#include <core/Context.h>

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
    //pipe->moveToThread(this);
    connect(pipe, SIGNAL(dataReceived(PipePackage)), this, SLOT(pipePackage(PipePackage)));
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
//            if (!p->isOpen())
//            {
//                if (!p->open())
//                {
//                    qDebug() << "Pipe" << (int)p << "connection failed!";
//                    if (p->connectionsFails() > 5)
//                    {
//                        //_pipeList.removeOne(p);
//                        //p->terminate();
//                        //delete p;
//                    }
//                }
//                else
//                {
//                    qDebug() << "Connection established!";
//                }
//            }
//            else
//            {
//                //p->ping();
//            }
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

void PipeController::pipePackage(PipePackage pkg)
{
    if (pkg.command() == PACKAGE_COMMAND_SHARE_REQUEST)
    {
        QString data;
        data.append(ip + ";");
        send(PipePackage(PACKAGE_COMMAND_SHARE_RESPONSE, data.toUtf8()));
    }
	else if (pkg.command() == PACKAGE_COMMAND_NEW_BLOCK)
	{
		qDebug() << "New block received!";
	}
	else
    {
        emit pipeCommand(QString::fromUtf8(pkg.data()));
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

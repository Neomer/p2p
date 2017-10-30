#include "MessageController.h"
#include <QDebug>
#include <iostream>
#include "core/Context.h"

MessageController::MessageController(QObject *parent) :
    QThread(parent)
{
    connect(Context::instance().keyLogger, SIGNAL(command(QString)), this, SLOT(addCommand(QString)));
    connect(Context::instance().pipeController, SIGNAL(pipeCommand(QString)), this, SLOT(addRemoteCommand(QString)));

	
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
    if (Context::instance().pipeController)
    {
        PipePackage pkg(PACKAGE_COMMAND_TEST_MESSAGE, message.toUtf8());
        Context::instance().pipeController->send(pkg);
    }
}

void MessageController::addRemoteCommand(QString message)
{
    QByteArray ba = QCryptographicHash::hash(message.toUtf8(), QCryptographicHash::Sha256);
    if (_history.contains(ba))
        return;

    _history << ba;
	
	
	
    std::cout << "Message received: " << message.toUtf8().constData() << "\n";
    if (Context::instance().pipeController)
    {
        PipePackage pkg(PACKAGE_COMMAND_TEST_MESSAGE, message.toUtf8());
        Context::instance().pipeController->send(pkg);
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
            if (Context::instance().pipeController)
            {
                //pipeController->send(m);
            }
        }

        msleep(1000);
    }
}


bool MessageController::onEventCatch(void *bus, QString event, QVariant data)
{
	Q_UNUSED(bus); Q_UNUSED(event); Q_UNUSED(data);
	return false;
}

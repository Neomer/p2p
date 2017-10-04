#include "KeyLogger.h"
#include <QDebug>
#include <QCoreApplication>
#include <iostream>

KeyLogger::KeyLogger(QObject *parent) :
    QThread(parent)
{
    _stream = new QTextStream(stdin);
    start(LowPriority);
}

void KeyLogger::terminate()
{
    qDebug() << "KeyLogger() - Thread stopped.";
    QThread::terminate();

}

void KeyLogger::run()
{
    qDebug() << "KeyLogger - thread started...";

    forever
    {
        QString sLine = _stream->readLine();
        if (sLine == "exit")
        {
            qApp->quit();
            return;
        }
        emit command(sLine);
    }
}

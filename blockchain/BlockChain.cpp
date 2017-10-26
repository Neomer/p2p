#include "BlockChain.h"
#include <QDebug>
#include <QFile>

BlockChain::BlockChain()
{
    
}

void BlockChain::load()
{
    QFile f(Context::instance().databasePath().absoluteFilePath("DB"));
    if (f.exists())
    {
        if (!f.open(QIODevice::ReadOnly))
        {
            throw std::runtime_error("Database file is locked!");
        }
        QJsonParseError err;
        QJsonDocument j = QJsonDocument::fromJson(f.readAll(), &err);
        if (err.error != QJsonParseError::NoError)
        {
            qDebug() << "Json parsing error!" << err.errorString();
            throw std::runtime_error("Json parsing error!");
        }
        ISerializable::deserialize(j.object());
        f.close();
    }
    
    Context::instance().busMain.subscribe("block.new", this);
}

bool BlockChain::find(Block *b, Hash h)
{
    QString path = getPathFromHash(h).join('/');
    QDir d = Context::instance().databasePath();
    if (!d.cd(path))
    {
        b = 0;
        return false;
    }
    QFile f(d.absoluteFilePath(h.toString()));
    if (!f.open(QIODevice::ReadOnly))
    {
        b = 0;
        return false;
    }
    QJsonParseError err;
    QJsonDocument j = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError)
    {
        qDebug() << "Json parsing error!" << err.errorString();
        b = 0;
        return false;
    }
    b->deserialize(j.object());
    f.close();
    return true;
}

bool BlockChain::appendBlock(Block *b)
{
    if (!contains(b->getPreviousBlock()))
    {
        qDebug() << "Previous block not found!" << b->getPreviousBlock();
        return false;
    }

    QJsonObject o = b->serialize();
    
    if (contains(o["hash"].toString()))
    {
        qDebug() << "Dublicate block!" << o["hash"].toString();
        return false;
    }
    
    QString path = getPathFromHash(Hash(o["hash"].toString())).join('/');
    QDir d = Context::instance().databasePath();
    if (!d.cd(path))
    {
        if (!d.mkpath(path))
        {
            qDebug() << "Can't create path" << path;
            return false;
        }
        if (!d.cd(path))
        {
            qDebug() << "Can't move to path" << path;
            return false;
        }
    }
    QFile f(d.absoluteFilePath(o["hash"].toString()));
    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can't create file" << path;
        return false;
    }
    QJsonDocument j(o);
    f.write(j.toJson(QJsonDocument::Indented));
    f.close();
    
    setLastBlockNumber(b->getNumber());
    setLastBlockHash(o["hash"].toString());
    ISerializable::save(Context::instance().databasePath().absoluteFilePath("DB"), ISerializable::serialize(false));
    return true;
}

bool BlockChain::contains(Hash h)
{
    QString path = getPathFromHash(h).join('/');
    QDir d = Context::instance().databasePath();
    if (!d.cd(path))
    {
        return false;
    }
    return QFile::exists(d.absoluteFilePath(h.toString()));
}
 
QStringList BlockChain::getPathFromHash(Hash h)
{
    QStringList ret;
    QString sHash = h.toString();
    for (int i = 0; i < 0; i += 2)
    {
        ret << sHash.mid(i, 2);
    }
    return ret;
}

bool BlockChain::onEventCatch(void *bus, QString event, QVariant data)
{
    if (bus == &(Context::instance().busNetwork))
    {
    }
    else if (bus == &(Context::instance().busMain))
    {
        if (event == "block.new")
        {
            Block *b = (Block *)data.toInt();
            save(b->serialize()["hash"].toString(), b->serialize());
        }
    }
}

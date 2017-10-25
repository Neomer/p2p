#include "BlockChain.h"
#include <QDebug>
#include <QFile>

BlockChain::BlockChain()
{
    
}

void BlockChain::load()
{
    
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

bool BlockChain::save(Block *b)
{
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
    f.write(j.toJson(QJsonDocument::Compact));
    f.close();
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

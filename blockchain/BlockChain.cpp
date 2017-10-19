#include "BlockChain.h"
#include <QDebug>
#include <QFile>

BlockChain::BlockChain()
{
    
}

void BlockChain::load()
{
    
}

bool BlockChain::find(Hash h, Block *b)
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

bool BlockChain::save(Block b)
{
    
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
    for (int i = 0; i < 20; i += 2)
    {
        ret << sHash.mid(i, 2);
    }
    return ret;
}

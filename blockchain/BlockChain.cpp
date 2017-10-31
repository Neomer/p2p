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
    
	if (getDifficulty().isEmpty())
	{
		setDifficulty("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
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
//	/qDebug() << d.absolutePath();
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
	updateDifficulty();
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

void BlockChain::updateDifficulty()
{
	if (getLastBlockNumber() <= 5)
	{
		return;
	}
	
	double avg_interval = 0;
	float idx_table[5] = {0.30f, 0.25f, 0.20f, 0.15f, 0.10f};
	Block b, b1;
	if (!find(&b, getLastBlockHash()))
	{
		throw std::runtime_error("Block-chain is corrupted!");
	}
	QDateTime time = time = b.getCreationTime();
	
	for (int i = 0; i < 5; i++)
	{
		if (!find(&b1, b.getPreviousBlock()))
		{
			throw std::runtime_error("Block-chain is corrupted!");
		}
		avg_interval += b1.getCreationTime().secsTo(b.getCreationTime()) * idx_table[i];
		b = b1;
	}
	qDebug() << "Diff factor:" << avg_interval;
}

bool BlockChain::onEventCatch(void *bus, QString event, QVariant data)
{
	//qDebug() << event << data;
    if (bus == &(Context::instance().busNetwork))
    {
    }
    else if (bus == &(Context::instance().busMain))
    {
        if (event == "block.new")
        {
            Block *b = (Block *)(qint64)data.toInt();
            BlockChain::appendBlock(b);
			return true;
        }
    }
	return false;
}

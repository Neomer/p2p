#include "Hash.h"
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QDateTime>

Hash::Hash()
{
    _hash.clear();
}

Hash::Hash(QByteArray hash)
{
    _hash = hash;
}

Hash::Hash(QString hash)
{
    _hash = QByteArray::fromHex(hash.toUtf8());
}

Hash::Hash(const char *hash)
{
    _hash = QByteArray::fromHex(QString(hash).toUtf8());
}

Hash::Hash(const Hash &other) :
    QObject()
{
    _hash = other._hash;
}

bool Hash::isHashOfEmpty()
{
    return _hash == QByteArray::fromHex(QString("a69f73cca23a9ac5c8b567dc185a756e97c982164fe25859e0d1dcc1475c80a615b2123af1f5f94c11e3e9402c3ac558f500199d95b6d3e301758586281dcd26").toUtf8());
}

Hash Hash::randomHash()
{
    return Hash(QCryptographicHash::hash(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz").toUtf8(), QCryptographicHash::Sha512));;
}

Hash Hash::hash(const QJsonObject &object)
{
    QJsonDocument json(object);
    return Hash(QCryptographicHash::hash(json.toJson(QJsonDocument::Compact), QCryptographicHash::Sha512));
}

Hash Hash::hash(ISerializable *object)
{
    return Hash::hash(object->serialize(false));
}

bool Hash::operator ==(const Hash &other)
{
    return _hash == other._hash;
}

bool Hash::operator ==(QByteArray hash)
{
    return _hash == hash;
}

bool Hash::operator ==(QString hash)
{
    return _hash == QByteArray::fromHex(hash.toUtf8());
}

bool Hash::operator !=(const Hash &other)
{
    return _hash != other._hash;
}

bool Hash::operator !=(QByteArray hash)
{
    return _hash != hash;
}

bool Hash::operator !=(QString hash)
{
    return _hash != QByteArray::fromHex(hash.toUtf8());
}

bool Hash::operator >(const Hash &other)
{
    return _hash > other._hash;
}

bool Hash::operator >(QByteArray hash)
{
    return _hash > hash;
}

bool Hash::operator >(QString hash)
{
    return _hash > QByteArray::fromHex(hash.toUtf8());
}

bool Hash::operator <(const Hash &other)
{
    return _hash < other._hash;
}

bool Hash::operator <(QByteArray hash)
{
    return _hash < hash;
}

bool Hash::operator <(QString hash)
{
    return _hash < QByteArray::fromHex(hash.toUtf8());
}

void Hash::operator =(const Hash &other)
{
    _hash = other._hash;
}

void Hash::operator =(QByteArray hash)
{
    _hash = hash;
}

void Hash::operator =(QString hash)
{
    _hash = QByteArray::fromHex(hash.toUtf8());
}

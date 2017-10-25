#include "ISerializable.h"
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <QDateTime>
#include <QCryptographicHash>
#include "core/Hash.h"

ISerializable::ISerializable(QObject *parent) : 
    QObject(parent)
{
    
}

QJsonObject ISerializable::serialize(bool hashed)
{
    QJsonObject ret;
    
    const QMetaObject *o = this->metaObject();
    if (!o)
    {
        qDebug() << "Meta object creation failed!";
        return ret;
    }
    for (int idx = o->propertyOffset(); idx < o->propertyCount(); ++idx)
    {
        QMetaProperty p = o->property(idx);
        if (!p.isReadable() || !p.isStored())
        {
            continue;
        }
        ret[QString(p.name())] = ISerializable::toJsonValue(p.read(this));
    }
    
    if (hashed)
    {
        // adding hash
        ret["hash"] = Hash::hash(ret).toString();
    }
    
    return ret;
}

bool ISerializable::deserialize(QJsonObject object)
{
    const QMetaObject *o = this->metaObject();
    if (!o)
    {
        qDebug() << "Meta object creation failed!";
        return false;
    }
    for (int idx = o->propertyOffset(); idx < o->propertyCount(); ++idx)
    {
        QMetaProperty p = o->property(idx);
        if (!object.contains(QString(p.name())))
        {
            continue;
        }
        if (!p.isWritable() || !p.isStored())
        {
            continue;
        }
        if (!p.write(this, ISerializable::fromJsonValue(object[QString(p.name())])))
        {
            qDebug() << "Property value didn't set!";
        }
    }
    
    if (object.contains("hash"))
    {
        Hash h = object.take("hash").toString();
        Hash hObj = Hash::hash(object);
        if (h != hObj)
        {
            qDebug() << "Hashes not equal!" << h.toString() << hObj.toString();
            return false;
        }
    }
    return true;
}

QJsonValue ISerializable::toJsonValue(QVariant value)
{
    switch (value.type())
    {
        case QVariant::Int: case QVariant::LongLong: 
        case QVariant::UInt: case QVariant::ULongLong:
            return value.toLongLong();
        
        case QVariant::Double:
            return value.toDouble();
            
        case QVariant::Bool:
            return value.toBool();
           
        case QVariant::DateTime:
            return value.toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
            
        case QVariant::Date:
            return value.toDate().toString("yyyy-MM-dd");
            
        case QVariant::Time:
            return value.toTime().toString("hh:mm:ss.zzz t");
            
        default:
            return value.toString();
    }
}

QVariant ISerializable::fromJsonValue(QJsonValue value)
{
    switch (value.type())
    {
            
        case QJsonValue::Bool:
            return value.toBool();
            
        case QJsonValue::Double:
            return value.toDouble();
            
        case QJsonValue::String:
            return value.toString();
        
        default:
            return QVariant();
    }
}

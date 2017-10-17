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
        if (!p.isReadable())
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

void ISerializable::deserialize(QJsonObject object)
{
    
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
            return value.toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz t");
            
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

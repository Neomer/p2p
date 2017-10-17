#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <QObject>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariant>

class ISerializable : public QObject
{
    Q_OBJECT
    
public:
    ISerializable(QObject *parent = 0);
    
    virtual QJsonObject serialize(bool hashed = true);
    virtual void deserialize(QJsonObject object);
    
    static QJsonValue toJsonValue(QVariant value);
    static QVariant fromJsonValue(QJsonValue value);
};

#endif // SERIALIZE_H

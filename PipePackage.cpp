#include "PipePackage.h"
#include <QCryptographicHash>
#include <QDebug>

PipePackage::PipePackage(unsigned short command, QByteArray data)
{
    _data = data;
	_raw = data;
	_raw.insert(0, (const char *)&command, sizeof(command));
	_cmd = command;
    _valid = true;
    //_raw.resize(data.size() + sizeof(command));
    //memcpy(_raw.data(), &command, sizeof(command));
    //memcpy(_raw.data() + sizeof(command), data.constData(), data.size());
}

PipePackage::PipePackage(QByteArray data)
{
    memcpy(&_length, data.constData(), sizeof(_length));
    if (_length > data.size() || _length < 0)
    {
        _valid = false;
        qDebug() << "Wrong package length";
    }
    memcpy(&_cmd , data.constData() + sizeof(_length), sizeof(_cmd));
    _data = QByteArray(data.constData() + sizeof(_length) + sizeof(_cmd), _length);
    _raw = QByteArray(data.constData() + sizeof(_length), _length + sizeof(_cmd));
    
    if (QCryptographicHash::hash(_data, QCryptographicHash::Sha256) != QByteArray(data.constData() + sizeof(_length) + sizeof(_cmd) + _length, 32))
    {
        _valid = false;
        qDebug() << "Wrong package hash";
    }
    
    _valid = true;
}

QByteArray PipePackage::hashed()
{
    QByteArray ret = _raw;
    int len = _data.length();
    ret.insert(0, (const char *)&len, sizeof(len));
    ret.append(QCryptographicHash::hash(_data, QCryptographicHash::Sha256));
    return ret;
}

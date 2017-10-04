#include "PipePackage.h"

PipePackage::PipePackage(short command, QByteArray data)
{
    _data = data;
    _raw.resize(data.size() + sizeof(command));
    memcpy(_raw.data(), &command, sizeof(command));
    memcpy(_raw.data() + sizeof(command), data.constData(), data.size());
}

PipePackage::PipePackage(QByteArray data)
{
    memcpy(&_cmd , data.constData(), sizeof(_cmd));
    _data = data.right(data.size() - sizeof(_cmd));
    _raw = _data;
}

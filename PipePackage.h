#ifndef PIPEPACKAGE_H
#define PIPEPACKAGE_H

#include <QByteArray>

#define PACKAGE_COMMAND_PING_REQUEST         0x0000
#define PACKAGE_COMMAND_PING_RESPONSE        0x0001
#define PACKAGE_COMMAND_SHARE_REQUEST        0x0002
#define PACKAGE_COMMAND_SHARE_RESPONSE       0x0003
#define PACKAGE_COMMAND_TEST_MESSAGE         0xffff

class PipePackage
{
public:
    PipePackage(short command, QByteArray data = QByteArray());
    PipePackage(QByteArray data);

    bool isValid() { return true; }
    short command() { return _cmd; }
    QByteArray data() { return _data; }
    QByteArray rawData() { return _raw; }

private:
    short _cmd;
    QByteArray _data, _raw;
};

#endif // PIPEPACKAGE_H

#ifndef PIPEPACKAGE_H
#define PIPEPACKAGE_H

#include <QByteArray>

#define PACKAGE_COMMAND_PING_REQUEST         0x0000
#define PACKAGE_COMMAND_PING_RESPONSE        0x0001
#define PACKAGE_COMMAND_SHARE_REQUEST        0x0002 // share nodes
#define PACKAGE_COMMAND_SHARE_RESPONSE       0x0003
#define PACKAGE_COMMAND_BLOCKCHAIN_REQUEST   0x0004 // info about block-chain
#define PACKAGE_COMMAND_BLOCKCHAIN_RESPONSE  0x0005
#define PACKAGE_COMMAND_NEW_BLOCK            0xf000
#define PACKAGE_COMMAND_TEST_MESSAGE         0xffff

class PipePackage
{
public:
    PipePackage(unsigned short command, QByteArray data = QByteArray());
    PipePackage(QByteArray data);

    bool isValid() { return _valid; }
    unsigned short command() { return _cmd; }
    QByteArray data() { return _data; }
    QByteArray rawData() { return _raw; }
    QByteArray hashed();
    

private:
    unsigned short _cmd;
    int _length;
    bool _valid;
    QByteArray _data, _raw;
};

#endif // PIPEPACKAGE_H

#include "Pipe.h"


Pipe::Pipe(QTcpSocket *socket, QObject *parent) :
    QObject(parent),
    _connectionsFails(0)
{
    _socket = socket;
    _addr = _socket->peerAddress().toString();
    _pingTimer = new QTimer(this);
    _lastActive = QDateTime::currentDateTime();
    connect(_socket, SIGNAL(readyRead()), this ,SLOT(readData()));
    connect(_pingTimer, SIGNAL(timeout()), this, SLOT(ping()));

    _pingTimer->setInterval(10000);
    _pingTimer->start();

    if (isClosed()) open();
}

Pipe::Pipe(QString address, QObject *parent) :
    QObject(parent),
    _connectionsFails(0)
{
    qDebug() << "Connect to" << address;
    _socket = new QTcpSocket(this);
    _addr = address;
    _pingTimer = new QTimer(this);
    _lastActive = QDateTime::currentDateTime();
    connect(_socket, SIGNAL(readyRead()), this ,SLOT(readData()));
    connect(_pingTimer, SIGNAL(timeout()), this, SLOT(ping()));

    _pingTimer->setInterval(10000);
    _pingTimer->start();

    if (isClosed()) open();
}

Pipe::~Pipe()
{
    _pingTimer->stop();
    delete _pingTimer;
}

void Pipe::flushBuffer()
{
    _buffer.clear();
}

bool Pipe::isOpen()
{
    return _socket->state() == QTcpSocket::ConnectedState;
}

bool Pipe::isClosed()
{
    return _socket->state() == QAbstractSocket::UnconnectedState;
}

bool Pipe::open()
{
    _socket->connectToHost(_addr, 15065);
    bool ret = _socket->waitForConnected(100);
    if (!ret)
    {
        _connectionsFails++;
    }
    else
    {
        _connectionsFails = 0;
    }
    return ret;
}

void Pipe::send(PipePackage data)
{
    qDebug() << "Pipe" << (int)this << "send to" << _socket->peerAddress().toString() << data.rawData().count() << "byte(s)";
    _socket->write(data.rawData());
}

void Pipe::ping()
{
    qDebug() << "Pipe" << (int)this << "send ping request";
    PipePackage resp(PACKAGE_COMMAND_PING_REQUEST);
    if (isClosed()) open();
    _socket->write(resp.rawData());
    _connectionsFails++;
}

void Pipe::readData()
{
    PipePackage pkg(_socket->readAll());
    if (pkg.command() == PACKAGE_COMMAND_PING_REQUEST)
    {
        qDebug() << "Pipe" << (int)this << "receive ping request";
        PipePackage resp(PACKAGE_COMMAND_PING_RESPONSE);
        _lastActive = QDateTime::currentDateTime();
        send(resp.rawData());
        _connectionsFails = 0;
    }
    else if (pkg.command() == PACKAGE_COMMAND_PING_RESPONSE)
    {
        qDebug() << "Pipe" << (int)this << "receive ping response";
        _lastActive = QDateTime::currentDateTime();
        _connectionsFails = 0;
    }
    else
    {
        qDebug() << "Pipe" << (int)this << "received from" << _socket->peerAddress().toString() << "command" << pkg.command();
        emit dataReceived(pkg);
    }
}


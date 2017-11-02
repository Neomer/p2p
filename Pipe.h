#ifndef PIPE_H
#define PIPE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QTimer>
#include <memory>

#include "PipePackage.h"

class Pipe : public QObject
{
    Q_OBJECT

public:
    Pipe(QTcpSocket *socket, QObject *parent = nullptr);
    Pipe(QString address, QObject *parent = 0);
    ~Pipe();

    void flushBuffer();
    bool isOpen();
    bool isClosed();
    bool open();
    void send(PipePackage data);

    int connectionsFails() { return _connectionsFails; }

private slots:
    void readData();
    void ping();

signals:
    void packageReceived(std::shared_ptr<PipePackage>);

private:
    QDateTime _lastActive;
    QTcpSocket *_socket;
    QByteArray _buffer;
    QString _addr;
    quint16 _port;
    int _connectionsFails;
    QTimer *_pingTimer;

    QList<QByteArray> _msgs;
};

#endif // PIPE_H

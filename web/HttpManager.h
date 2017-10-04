#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QUrl>
#include <QTcpSocket>
#include <QSslSocket>

class HttpManager : public QObject
{
    Q_OBJECT

public:
    enum HTTPMethod
    {
        enMethodGET,
        enMethodPOST,
        enMethodPUT,
        enMethodDELETE,
        enMethodHEAD
    };

    explicit HttpManager(QObject *parent = nullptr);

    static QString getPage(QUrl url, HTTPMethod method = enMethodGET);


};

#endif // HTTPMANAGER_H

#ifndef HTTPUTILS_H
#define HTTPUTILS_H

#include <QObject>

class HttpUtils : public QObject
{
    Q_OBJECT
public:
    explicit HttpUtils(QObject *parent = nullptr);

    static QString fromChunkedTransferEncoding(QString data);

};

#endif // HTTPUTILS_H

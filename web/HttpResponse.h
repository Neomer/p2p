#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <QUrl>
#include <QMap>
#include <QList>
#include <zlib.h>

class HttpResponse
{
public:
    HttpResponse(QString response);

    void clearHeaders();
    void addHeader(QString key, QString value);

    int status() { return _status; }
    QString statusMessage() { return _statusDescription; }
    QString body() { return _body; }
    QString header(QString key);
    bool hasHeader(QString key);

private:
    void parseHttpResponse(QString value);

    QMap<QString, QString> _headers;
    QString _body, _statusDescription;
    int _status;
};

#endif // HTTPRESPONSE_H

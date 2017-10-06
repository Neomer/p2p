#include "HttpResponse.h"
#include <QDebug>
#include "web/HttpUtils.h"

HttpResponse::HttpResponse(QString response)
{
    parseHttpResponse(response);
    QString enc = header("Content-Encoding");
    if (!enc.isEmpty())
    {
        if (enc == "gzip")
        {

        }
        else if (enc == "deflate")
        {

        }
        else
        {
            throw std::runtime_error("Unknown compression type!");
        }
    }
    enc = header("Transfer-Encoding");
    if (!enc.isEmpty())
    {
        if (enc == "chunked")
        {
            _body = HttpUtils::fromChunkedTransferEncoding(_body);
        }
        else
        {
            throw std::runtime_error("Unknown Transfer-Encoding type!");
        }
    }
    else
    {
//        if (header("Content-Length").isEmpty())
//        {
//            throw std::runtime_error("Both Content-Length and Transfer-Encoding headers are missing!");
//        }
    }
}

void HttpResponse::clearHeaders()
{
    _headers.clear();
}

void HttpResponse::addHeader(QString key, QString value)
{
    _headers[key.toLower()] = value;
}

QString HttpResponse::header(QString key)
{
    if (hasHeader(key))
    {
        return _headers[key.toLower()];
    }
    else
    {
        return "";
    }
}

bool HttpResponse::hasHeader(QString key)
{
    return _headers.keys().contains(key.toLower());
}

void HttpResponse::parseHttpResponse(QString reply)
{
    QString sresult = reply;//QString(reply).replace("\r", "");

    clearHeaders();
    QString srow;
    int nextrow = sresult.indexOf("\r\n");
    int count = 0;
    while (nextrow >= 0)
    {
        srow = sresult.mid(0, nextrow);
        sresult = sresult.mid(nextrow + 2);

        // Проверяем на повторяющиеся <LF>
        if (srow.isEmpty())
        {
            // Если мы в самом начале, то пропускаем переносы каретки в начале пакета,
            // иначе мы прошли весь заголовок
            if (count > 0)
                break;
            else
                continue;
        }
        else
        {
            if (count == 0) // Парсим строку статуса ответа
            {
                QStringList statusrow = srow.split(' ', QString::SkipEmptyParts);
                if (statusrow.count() < 3)
                {
                    qDebug() << "Wrong format of status-row!" <<  srow;
                    throw std::runtime_error("Wrong format of status-row!");
                }
                bool ok = true;
                _status = statusrow.at(1).toInt(&ok);
                if (!ok)
                {
                    qDebug() << "Wrong format of status-row!" <<  srow;
                    throw std::runtime_error("Wrong format of status-row!");
                }
                _statusDescription = srow.mid(srow.indexOf(" ", srow.indexOf(" ") + 1) + 1);
            }
            else // Парсим заголовок
            {
                int delidx = srow.indexOf(':');
                if (delidx == -1)
                {
                    qDebug() << "Incorrect row!" << srow;
                    continue;
                }
                QString headerName = srow.mid(0, delidx).trimmed();
                QString headerData = srow.mid(delidx + 1).trimmed();
                addHeader(headerName, headerData);
            }
        }

        count++;
        nextrow = sresult.indexOf("\r\n");
    }
    _body = sresult;
}

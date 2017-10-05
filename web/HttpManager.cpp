#include "HttpManager.h"

HttpManager::HttpManager(QObject *parent) : QObject(parent)
{

}

QString HttpManager::getPage(QUrl url, HttpManager::HTTPMethod method)
{
    QString sMet = "";
    switch (method)
    {
        case enMethodGET:
            sMet = "GET";
            break;

        case enMethodPOST:
            sMet = "POST";
            break;

        case enMethodPUT:
            sMet = "PUT";
            break;

        case enMethodDELETE:
            sMet = "DELETE";
            break;

        case enMethodHEAD:
            sMet = "HEAD";
            break;

        default:
            throw std::runtime_error("Unknown request method!");
    }

    QString sRequest = sMet + " " + (url.path().isEmpty() ? "/" : url.path()) + (url.hasQuery() ? "?" + url.query() : "") + " HTTP/1.0\r\n" +
            "Host: " + url.host() + "\r\n" +
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n" +
            "Connection: keep-alive\r\n" +
            "Cache-Control: max-age=0\r\n" +
            "Accept-Encoding: gzip, deflate, identity\r\n" +
            "X-Compress: null\r\n" +
            "User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36\r\n\r\n";

    if (url.toString().indexOf("https://") == -1)
    {
        QTcpSocket socket;
        socket.connectToHost(url.host(), url.port(80));
        if (!socket.waitForConnected(3000))
        {
            throw std::runtime_error("Server unavailable!");
        }
        if (socket.write(sRequest.toUtf8()) != sRequest.length())
        {
            throw std::runtime_error("Not all data was written!");
        }
        QString ret;
        while (socket.waitForReadyRead(5000))
        {
            ret.append(QString::fromUtf8(socket.readAll()));
        }

        socket.close();
        return ret;
    }
    else
    {
        QSslSocket socket;
        socket.connectToHostEncrypted(url.host(), url.port(443));
        if (!socket.waitForConnected(3000))
        {
            throw std::runtime_error("Server unavailable!");
        }
        if (socket.write(sRequest.toUtf8()) != sRequest.length())
        {
            throw std::runtime_error("Not all data was written!");
        }
        QString ret;
        while (socket.waitForReadyRead(5000))
        {
            ret.append(QString::fromUtf8(socket.readAll()));
        }
        socket.close();
        return ret;
    }
}

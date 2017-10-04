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

    QString sRequest = sMet + " " + (url.path().isEmpty() ? "/" : url.path()) + (url.hasQuery() ? "?" + url.query() : "") + " HTTP/1.1\nHost: " + url.host() + "\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\nConnection: keep-alive\nCache-Control: max-age=0\nAccept-Encoding: identity, gzip\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36\n\n";
    //QString sRequest = "GET /?format=text HTTP/1.1\nHost: api.ipify.org\nContent-Length: 0\n\n";

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
        while (socket.waitForReadyRead(1000))
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
        if (!socket.waitForReadyRead(5000))
        {
            throw std::runtime_error("Timeout!");
        }
        QString ret = QString::fromUtf8(socket.readAll());
        socket.close();
        return ret;
    }
}

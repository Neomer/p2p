#include "HttpUtils.h"

HttpUtils::HttpUtils(QObject *parent) : QObject(parent)
{

}

QString HttpUtils::fromChunkedTransferEncoding(QString data)
{
    QString ret = "";
    int idx = data.indexOf("\r\n");

    while (idx > 0)
    {
        bool ok = true;
        int chunkLength = data.mid(0, idx).toInt(&ok, 16);
        if (!ok)
        {
            throw std::runtime_error("Value is not a valid number!");
        }
        ret.append(data.mid(idx + 2, chunkLength));
        idx = data.indexOf("\r\n", idx + chunkLength + 2);
    }
    return ret;
}

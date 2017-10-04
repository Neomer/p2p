#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>

class HttpManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HTTPMANAGER_H
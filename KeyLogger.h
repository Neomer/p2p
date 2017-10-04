#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <QObject>
#include <QThread>
#include <QTextStream>

class KeyLogger : public QThread
{
    Q_OBJECT
public:
    explicit KeyLogger(QObject *parent = nullptr);

    void terminate();

signals:
    void command(QString);

public slots:

    // QThread interface
protected:
    void run();

private:
    QTextStream *_stream;
};

#endif // KEYLOGGER_H

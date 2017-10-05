#ifndef KEYCOMMAND_H
#define KEYCOMMAND_H

#include <QObject>
#include <QList>

class KeyCommand
{
public:
    explicit KeyCommand(QString data);

    QString className() { return _class; }
    QString methodName() { return _method; }
    QList<QString> args() { return _args; }

private:
    QString _class, _method;
    QList<QString> _args;
};

#endif // KEYCOMMAND_H

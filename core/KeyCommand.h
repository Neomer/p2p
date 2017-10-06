#ifndef KEYCOMMAND_H
#define KEYCOMMAND_H

#include <QObject>
#include <QList>

class KeyCommand
{
public:
    KeyCommand();
    explicit KeyCommand(QString data);
    KeyCommand(const KeyCommand &other);

    QString className() { return _class; }
    QString methodName() { return _method; }
    QList<QString> args() { return _args; }

private:
    QString _class, _method;
    QList<QString> _args;
};

Q_DECLARE_METATYPE(KeyCommand)

#endif // KEYCOMMAND_H

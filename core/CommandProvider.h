#ifndef COMMANDPROVIDER_H
#define COMMANDPROVIDER_H

#include <QObject>
#include <QMap>

#include "KeyCommand.h"

class CommandProvider : public QObject
{
    Q_OBJECT

public:
    static CommandProvider &instance()
    {
        static CommandProvider i;
        return i;
    }

    void registerCommand(QObject *object, QString name);
    bool call(KeyCommand command);

private:
    CommandProvider(QObject *parent = nullptr);
    CommandProvider(const CommandProvider &other);
    CommandProvider &operator = (const CommandProvider &other);

    QMap<QString, QObject *> _cmds;
};

#endif // COMMANDPROVIDER_H

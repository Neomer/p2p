#include "CommandProvider.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaMethod>
#include <QDebug>

void CommandProvider::registerCommand(QObject *object, QString name)
{
    _cmds[name.toLower()] = object;
}

bool CommandProvider::call(KeyCommand command)
{
    if (_cmds.contains(command.className().toLower()))
    {
        QObject *o = _cmds[command.className().toLower()];
        const QMetaObject *meta = o->metaObject();
        for (int idx = meta->methodOffset(); idx < meta->methodCount(); idx++)
        {
            QMetaMethod meth = meta->method(idx);
            if (QString::fromUtf8(meth.name()).toLower() == command.methodName().toLower())
            {
                if (meth.access() != QMetaMethod::Public || meth.methodType() != QMetaMethod::Slot) return false;
                if (!meth.invoke(o, Qt::DirectConnection)) 
                {
                    qDebug() << "Invoke failed!" << command.className() << command.methodName() << command.args().count();
                    throw std::runtime_error("Invoke failed!");
                }
                return true;
            }
        }
    }
    return false;
}

CommandProvider::CommandProvider(QObject *parent) : QObject(parent)
{

}

#include "KeyCommand.h"

KeyCommand::KeyCommand()
{
    _class = "";
    _method = "";
    
}

KeyCommand::KeyCommand(QString data)
{
    data = data.trimmed();
    int pnt = data.indexOf('.');
    _class = data.mid(0, pnt);
    _method = data.mid(pnt + 1, data.indexOf('(') - pnt - 1);
    pnt = data.indexOf('(');
    _args = data.mid(pnt + 1, data.lastIndexOf(')') - pnt - 1).split(',', QString::SkipEmptyParts);
}

KeyCommand::KeyCommand(const KeyCommand &other)
{
    _method = other._method;
    _class = other._class;
    _args = other._args;
}

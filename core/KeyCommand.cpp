#include "KeyCommand.h"

KeyCommand::KeyCommand(QString data)
{
    data = data.trimmed();
    int pnt = data.indexOf('.');
    _class = data.mid(0, pnt);
    _method = data.mid(pnt + 1, data.indexOf('(') - pnt - 1);
    pnt = data.indexOf('(');
    _args = data.mid(pnt + 1, data.lastIndexOf(')') - pnt - 1).split(',');
}

#ifndef PIPECONTROLLER_H
#define PIPECONTROLLER_H

#include <QThread>
#include "Gateway.h"
#include "core/KeyCommand.h"

class PipeController : public QThread
{
    Q_OBJECT

public:
    PipeController(Gateway *gateway, QObject * parent = 0);

    void terminate();
    void send(PipePackage data);

public slots:
    void addPipe(Pipe *pipe);
    
// Key Commands
    void getPipeCount();
    void createPipe(KeyCommand command);

protected:
    void run();

signals:
    void pipeCommand(QString);

private slots:
    void pipePackage(PipePackage);

private:
    QList<Pipe *> _pipeList;
};

#endif // PIPECONTROLLER_H

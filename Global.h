#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>

#include "KeyLogger.h"
#include "PipeController.h"
#include "MessageController.h"


static KeyLogger *keyLogger = 0;
static PipeController *pipeController = 0;
static MessageController *messageController = 0;

#endif // GLOBAL_H

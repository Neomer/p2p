#include "Context.h"
#include <QStandardPaths>

QDir Context::databasePath()
{
    return QDir(appDataPath().absoluteFilePath("db"));
}

QDir Context::appDataPath()
{
    return QDir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(0));
}

void Context::load()
{
    if (!appDataPath().exists("db"))
    {
        if (!appDataPath().mkpath("db"))
        {
            throw std::runtime_error("Database folder creation failed!");
        }
    }
}

Context::Context(QObject *parent) : 
    QObject(parent)
{
    
}

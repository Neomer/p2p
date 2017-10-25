#include "Block.h"

Block::Block(QObject *parent) :
    ISerializable(parent),
    m_creationTime(QDateTime::currentDateTime())
{
    
}

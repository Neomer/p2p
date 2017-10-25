#include "Block.h"

Block::Block(QObject *parent) :
    ISerializable(parent),
    m_creationTime(QDateTime::currentDateTime()),
    m_number(0),
    m_data(""),
    m_nonce(0),
    m_confirmed(false)
{
    
}

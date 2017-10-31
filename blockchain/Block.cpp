#include "Block.h"

Block::Block(QObject *parent) :
    ISerializable(parent),
    m_creationTime(QDateTime::currentDateTime()),
    m_number(0),
    m_data(""),
    m_nonce(0),
    m_confirmed(false),
    m_version(1)
{
	
}

Block &Block::operator =(const Block &other)
{
	m_creationTime = other.m_creationTime;
    m_number = other.m_number;
    m_data = other.m_data;
    m_nonce = other.m_nonce;
    m_confirmed = other.m_confirmed;
    m_version = other.m_version;
	return *this;
}

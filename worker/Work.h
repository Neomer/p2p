#ifndef WORK_H
#define WORK_H

#include <QDateTime>

#include <blockchain/Block.h>

class Work
{
public:
	Work(Block *block, Hash goal);
	Work(const Work &other);
	
	QDateTime createDateTime() { return _createDateTime; }
	Hash goal() { return _goal; }
	Block *block() { return _block; }
	
private:
	QDateTime _createDateTime;
	Hash _goal;
	Block *_block;
};

#endif // WORK_H

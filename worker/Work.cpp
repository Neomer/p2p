#include "Work.h"

Work::Work(Block *block, Hash goal) :
	_block(block),
	_goal(goal)
{
	_createDateTime = QDateTime::currentDateTime();
}


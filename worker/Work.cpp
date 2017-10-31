#include "Work.h"

Work::Work(Block *block, Hash goal) :
	_block(block),
	_goal(goal)
{
	_createDateTime = QDateTime::currentDateTime();
}

Work::Work(const Work &other)
{
	_block = other._block; 
	_goal = other._goal;
	_createDateTime = other._createDateTime;
}


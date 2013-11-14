#ifndef SWUtil_h__
#define SWUtil_h__

#include "SWMemory.h"

#define SWUtil (__SWUtil::getInstance())

class __SWUtil : public SWMemory
{
	__SWUtil();
	~__SWUtil();
public:
	static __SWUtil& getInstance();

	unsigned int getTickCount();

	void consoleXY( int x, int y );
};

#endif // SWUtil_h__
#include "SWUtil.h"

#include <stdlib.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

__SWUtil::__SWUtil()
{
}

__SWUtil::~__SWUtil()
{
}

__SWUtil& __SWUtil::getInstance()
{
	static __SWUtil instance;
	return instance;
}

unsigned int __SWUtil::getMicroCount()
{
#ifdef WIN32
    return GetTickCount()*1000;
#else
    struct timeval tick;
    gettimeofday(&tick, 0);
    unsigned int sec = tick.tv_sec*1000000;
    unsigned int usec = tick.tv_usec;
    return (sec + usec);
#endif
}

void* __SWUtil::alloc( size_t size )
{
	return malloc( size );
}

void  __SWUtil::free( void* memory )
{
	::free( memory );
}

void __SWUtil::consoleXY( int x, int y )
{
#ifdef WIN32
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#endif
}

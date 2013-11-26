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



unsigned int __SWUtil::getTickCount()

{
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval tick;
    gettimeofday(&tick, 0);
    return (tick.tv_sec*1000 + tick.tv_usec/1000);
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

	//COORD pos = {x, y};

	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}


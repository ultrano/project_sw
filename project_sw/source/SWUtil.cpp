#include "SWUtil.h"
#include <Windows.h>

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
	return GetTickCount();
}

void __SWUtil::consoleXY( int x, int y )
{
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

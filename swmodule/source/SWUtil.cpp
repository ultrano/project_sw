#include "SWUtil.h"

#include <locale>
#include <codecvt>
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

tnumber __SWUtil::strToNum( const tstring& str )
{
	float num = 0;
	sscanf( str.c_str(), "%f", &num );
	return num;
}

void __SWUtil::consoleXY( int x, int y )
{
#ifdef WIN32
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#endif
}

twstring __SWUtil::utf8ToUnicode( const tstring& str )
{
	std::wstring_convert< std::codecvt_utf8<wchar_t> > conv;
	const char* begin = &str[0];
	const char* end   = &str[str.size()];

	//! bom check
	if ( str.size() > 3 ) do
	{
		if ( str[0] != (char)0xEF ) break;
		if ( str[1] != (char)0xBB ) break;
		if ( str[2] != (char)0xBF ) break;
		begin = &str[3];
	} while ( false );

	return conv.from_bytes( begin, end );
}

tstring __SWUtil::unicodeToUtf8( const twstring& str )
{
	if ( str.size() == 0 ) return "";
	std::wstring_convert< std::codecvt_utf8<wchar_t> > conv;
	const wchar_t* begin = &str[0];
	const wchar_t* end   = &str[str.size()];
	
	//! bom check
	if ( str[0] == (wchar_t)0xfeff ) begin = &str[1];
	else if ( str[0] == (wchar_t)0xfffe ) begin = &str[1];

	return conv.to_bytes( begin, end );

}


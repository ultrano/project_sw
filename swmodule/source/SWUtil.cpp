#include "SWUtil.h"
#include "SWIOStream.h"

#include <locale>
#include <stdlib.h>

#ifdef _MSC_VER
#include <Windows.h>
#include <codecvt>
#else
#include <sys/time.h>
#include <iconv.h>
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
#ifdef _MSC_VER
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
	return SWAlloc( size );
	//return malloc( size );
}

void  __SWUtil::free( void* memory )
{
	SWFree( memory );
	//::free( memory );
}

tnumber __SWUtil::strToNum( const tstring& str )
{
	float num = 0;
	sscanf( str.c_str(), "%f", &num );
	return num;
}

void __SWUtil::consoleXY( int x, int y )
{
#ifdef _MSC_VER
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#endif
}

twstring __SWUtil::utf8ToUnicode( const tstring& str )
{
#ifdef _MSC_VER
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
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

	return conv.from_bytes( begin, end ).c_str();
#else
	return L"";
#endif
}

tstring __SWUtil::unicodeToUtf8( const twstring& str )
{
#ifdef _MSC_VER
	if ( str.size() == 0 ) return "";
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	const wchar_t* begin = &str[0];
	const wchar_t* end   = &str[str.size()];
	
	//! bom check
	if ( str[0] == (wchar_t)0xfeff ) begin = &str[1];
	else if ( str[0] == (wchar_t)0xfffe ) begin = &str[1];

	return conv.to_bytes( begin, end ).c_str();
#else
	return "";
#endif
}


void __SWUtil::copyStream( SWOutputStream* os, SWInputStream* is )
{
	if ( !os || !is ) return;

	int sz = is->available();
	if ( sz <= 0 ) return;

	tarray<tbyte> buf;
	buf.resize( sz );
	int ret = is->read( &buf[0], sz );
	if ( ret <= 0 ) return;
	os->write( &buf[0], sz );

}

thash32 __SWUtil::stringHash( const tstring& str )
{
	thash32 b    = 378551;
	thash32 a    = 63689;
	thash32 hash = 0;

	for(std::size_t i = 0; i < str.length(); i++)
	{
		hash = hash * a + str[i];
		a    = a * b;
	}

	return (hash & 0x7FFFFFFF);
}
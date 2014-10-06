#include "SWUtil.h"
#include "SWIOStream.h"

#include <locale>
#include <stdlib.h>


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

tdouble __SWUtil::strToNum( const tstring& str )
{
	float num = 0;
	sscanf( str.c_str(), "%f", &num );
	return num;
}
/*
bool __SWUtil::utf8ToUTF16( const tstring& utf8, twstring& unicode )
{
	iconv_t cd = iconv_open( "UTF-16", "UTF-8" );
	if ( cd == (iconv_t)-1 ) return false;

	tuint inSize  = utf8.size();
	tuint outSize = inSize * 2;
	const char* inBuf  = (char*)SWAlloc( inSize );
	char* outBuf = (char*)SWAlloc( outSize );

	memcpy( (void*)inBuf, utf8.c_str(), inSize );

	tuint ret = iconv( cd, &inBuf, &inSize, &outBuf, &outSize );
	if ( ret != (tuint)-1 ) unicode = (wchar_t*)outBuf;

	SWFree( (void*)inBuf );
	SWFree( (void*)outBuf );

	return ( ret != (tuint)-1 );
}

bool __SWUtil::utf16ToUTF8( const twstring& unicode, tstring& utf8 )
{
	iconv_t cd = iconv_open( "UTF-8", "UTF-16" );
	if ( cd == (iconv_t)-1 ) return false;

	tuint inSize  = unicode.size() * 2;
	tuint outSize = inSize * 2;
	const char* inBuf  = (char*)SWAlloc( inSize );
	char* outBuf = (char*)SWAlloc( outSize );

	memcpy( (void*)inBuf, unicode.c_str(), inSize );

	tuint ret = iconv( cd, &inBuf, &inSize, &outBuf, &outSize );
	if ( ret != (tuint)-1 ) utf8 = outBuf;

	SWFree( (void*)inBuf );
	SWFree( (void*)outBuf );

	return ( ret != (tuint)-1 );
	}
	*/


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

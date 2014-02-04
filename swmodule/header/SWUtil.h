#ifndef SWUtil_h__
#define SWUtil_h__

#include "SWMemory.h"
#include "SWRefCounter.h"
#include "SWString.h"
#include "SWNumber.h"

#define SWUtil (__SWUtil::getInstance())

class SWObject;
class SWInputStream;
class SWOutputStream;
class __SWUtil : public SWMemory
{
	__SWUtil();
	~__SWUtil();
    unsigned int tickCount;
public:
	static __SWUtil& getInstance();

	unsigned int getMicroCount();

	void* alloc( size_t size );
	void  free( void* memory );

	tnumber strToNum( const tstring& str );

	twstring utf8ToUnicode( const tstring& str );
	tstring  unicodeToUtf8( const twstring& str );

	void consoleXY( int x, int y );

	void copyStream( SWOutputStream* os, SWInputStream* is );

	thash32 stringHash( const tstring& str );
};

#endif // SWUtil_h__
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

	void* alloc( size_t size );
	void  free( void* memory );

	tnumber strToNum( const tstring& str );

	/*
	bool utf8ToUTF16( const tstring& utf8, twstring& unicode );
	bool utf16ToUTF8( const twstring& unicode, tstring& utf8 );
	*/

	void copyStream( SWOutputStream* os, SWInputStream* is );

	thash32 stringHash( const tstring& str );
};

#endif // SWUtil_h__
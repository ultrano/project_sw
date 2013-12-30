#ifndef SWUtil_h__
#define SWUtil_h__

#include "SWMemory.h"
#include "SWRefCounter.h"
#include "SWString.h"
#include "SWNumber.h"

#define SWUtil (__SWUtil::getInstance())

class SWObject;
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

	SWNumber::Value strToNum( const SWString::Value& str );

	void consoleXY( int x, int y );
};

#endif // SWUtil_h__
#ifndef Sparky_h__
#define Sparky_h__

#include "SWMemory.h"
#include "SWString.h"

class Sparky : public SWMemory
{
	void drawText( const SWString::Value& text );
};

#endif // Sparky_h__
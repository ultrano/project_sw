#ifndef Sparky_h__
#define Sparky_h__

#include "SWMemory.h"
#include "SWString.h"

class Sparky : public SWMemory
{
	void drawText( const tstring& text );
};

#endif // Sparky_h__
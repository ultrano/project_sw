#ifndef WIFontChar_h__
#define WIFontChar_h__

#include "SWRefCounter.h"

class WIFontChar : public SWRefCountable
{
public:
	WIFontChar() : id(0), x(0), y(0), w(0), h(0), offsetX(0), offsetY(0) {};
	~WIFontChar() {};
	int id;
	unsigned short x,y;
	unsigned short w,h;
	int offsetX, offsetY;
};

#endif // WIFontChar_h__
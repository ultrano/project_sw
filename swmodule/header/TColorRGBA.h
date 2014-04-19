#ifndef TColor_h__
#define TColor_h__

#include "SWPrimaryType.h"

class TColorRGBA : public SWMemory
{
public:
	float r,g,b,a;

	TColorRGBA() : r( 0 ), g( 0 ), b( 0 ), a( 0 ) {}
	TColorRGBA( float _r, float _g, float _b, float _a )
		: r( _r ), g( _g ), b( _b ), a( _a )
	{
	}

	void setRGBA( float _r, float _g, float _b, float _a ) { r = _r; g = _g; b = _b; a = _a; }
	void setRGB( float _r, float _g, float _b ) { r = _r; g = _g; b = _b; }
};

#endif // TColor_h__
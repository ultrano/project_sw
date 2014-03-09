#ifndef TRect_h__
#define TRect_h__

#include "SWPrimaryType.h"

class TRect : public SWMemory
{
public:
	float left;
	float top;
	float right;
	float bottom;

	TRect() : left(0), top(0), right(0), bottom(0) {}
	TRect( float _left, float _top, float _right, float _bottom )
		: left( _left ) , top( _top ) , right( _right ) , bottom( _bottom ) {}

	void setRect( float _left, float _top, float _right, float _bottom )
	{
		left   = _left;
		top    = _top;
		right  = _right;
		bottom = _bottom;
	}

	float width() const { return (right > left)? (right - left) : (left - right); }
	float height() const { return ( bottom > top )? (bottom - top) : (top - bottom); }
};

#endif // TRect_h__
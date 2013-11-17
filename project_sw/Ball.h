#ifndef Ball_h__
#define Ball_h__

#include "SWBehavior.h"
#include "SWVector3f.h"

class Ball : public SWBehavior
{
	SW_RTTI( Ball, SWBehavior );

public:
	
	SWVector3f velocity;

	void onAwake();
	void onUpdate();
};

#endif // Ball_h__
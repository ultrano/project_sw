#ifndef BallGenerator_h__
#define BallGenerator_h__

#include "SWBehavior.h"
#include "TVector2f.h"

class BallGenerator : public SWBehavior
{
	SW_RTTI( BallGenerator, SWBehavior );

public:
	float accum;
	TVector2f force;
	bool turnOn;

	void onAwake();
	void onUpdate();

};

#endif // BallGenerator_h__
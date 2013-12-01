#ifndef CatGenerator_h__
#define CatGenerator_h__

#include "SWBehavior.h"

class CatGenerator : public SWBehavior
{
	SW_RTTI( CatGenerator, SWBehavior );

	float genTime;
	float accum;
	void onAwake();
	void onUpdate();

};

#endif // CatGenerator_h__
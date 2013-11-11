#ifndef Cat_h__
#define Cat_h__

#include "SWBehavior.h"

class Cat : public SWBehavior
{
	SW_RTTI( Cat, SWBehavior );

public:
	float lifeTime;
	void onAwake();
	void onUpdate();
};

#endif // Cat_h__
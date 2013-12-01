#ifndef Cat_h__
#define Cat_h__

#include "SWBehavior.h"

class Cat : public SWBehavior
{
	SW_RTTI( Cat, SWBehavior );
	float m_speed;
public:
	void onAwake();
	void onUpdate();
};

#endif // Cat_h__
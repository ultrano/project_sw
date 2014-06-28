#ifndef Bird_h__
#define Bird_h__

#include "SWHeaders.h"

class Bird : public SWBehavior
{
	SW_RTTI( Bird, SWBehavior );

public:

	Bird( factory_constructor );

protected:

	void onAwake();
	void onUpdate();
	void onCollision( SWCollision2D* );

private:


};

#endif
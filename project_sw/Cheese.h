#ifndef Cheese_h__
#define Cheese_h__

#include "SWBehavior.h"

class Cheese : public SWBehavior
{
	SW_RTTI( Cheese, SWBehavior );

public:

	Cheese( factory_constructor );
	~Cheese();
	virtual void onAwake();
	virtual void onCollision( SWCollision2D* coll );
	virtual void onUpdate();
	void onDeleteGO();
};

#endif // Cheese_h__
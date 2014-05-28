#ifndef Mouse_h__
#define Mouse_h__

#include "SWBehavior.h"

class Mouse : public SWBehavior
{
	SW_RTTI( Mouse, SWBehavior );

public:

	Mouse( factory_constructor );
	~Mouse();
	virtual void onAwake();
	virtual void onCollision( SWCollision2D* coll );
	virtual void onUpdate();
};

#endif // Mouse_h__
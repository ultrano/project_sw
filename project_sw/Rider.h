#ifndef Rider_h__
#define Rider_h__

#include "SWHeaders.h"

class Rider : public SWBehavior
{
	SW_RTTI( Rider, SWBehavior );

public:
	
	Rider();
	Rider( factory_constructor );
	~Rider();

protected:

	virtual void onAwake();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedFrameUpdate();
	virtual void onCollision( SWCollision2D* );

private:



};

#endif //! Rider_h__
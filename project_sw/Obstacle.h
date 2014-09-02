#ifndef Obstacle_h__
#define Obstacle_h__

#include "SWHeaders.h"

class Obstacle : public SWBehavior
{
	SW_RTTI( Obstacle, SWBehavior );

public:
	
	Obstacle( factory_constructor );
	~Obstacle();

protected:

	virtual void onAwake();
	virtual void onCollision( SWCollision2D* );

private:


};

#endif //! Obstacle_h__
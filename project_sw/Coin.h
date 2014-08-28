#ifndef Coin_h__
#define Coin_h__

#include "SWHeaders.h"

class Coin : public SWBehavior
{
	SW_RTTI( Coin, SWBehavior );

public:
	
	Coin();
	Coin( factory_constructor );
	~Coin();

protected:
	
	virtual void onAwake();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

private:
};

#endif //! Coin_h__
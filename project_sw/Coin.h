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

	void deposit();
	void withDraw();

protected:
	
	virtual void onAwake();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

};

#endif //! Coin_h__
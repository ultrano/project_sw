#ifndef FireWork_h__
#define FireWork_h__

#include "SWHeaders.h"

class FireWork : public SWBehavior
{
	SW_RTTI( FireWork, SWBehavior );

public:

	FireWork( factory_constructor );
	~FireWork();

	static FireWork* createFireWork();

	void fire();

private:

	void goToPool();

protected:

	virtual void onAwake();
	virtual void onFixedRateUpdate();

	void onEndAction();

};


#endif // FireWork_h__
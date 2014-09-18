#ifndef FireWork_h__
#define FireWork_h__

#include "SWHeaders.h"

class FireWork : public SWBehavior
{
	SW_RTTI( FireWork, SWBehavior );

public:

	FireWork( factory_constructor );
	~FireWork();

protected:

	virtual void onStart();
	virtual void onFixedRateUpdate();

};


#endif // FireWork_h__
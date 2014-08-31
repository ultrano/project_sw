#ifndef GasCloud_h__
#define GasCloud_h__

#include "SWHeaders.h"

class GasCloud : public SWBehavior
{
	SW_RTTI( GasCloud, SWBehavior );

public:
	
	GasCloud();
	GasCloud( factory_constructor );
	~GasCloud();

	void reset( const tvec3& pos );

protected:
	
	virtual void onAwake();

private:

	void goToPool();

};

#endif //! GasCloud_h__
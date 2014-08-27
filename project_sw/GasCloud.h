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

protected:
	
	virtual void onAwake();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedFrameUpdate();
	virtual void onCollision( SWCollision2D* );

private:
};

#endif //! GasCloud_h__
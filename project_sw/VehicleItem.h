#ifndef VehicleItem_h__
#define VehicleItem_h__

#include "SWHeaders.h"

class VehicleItem : public SWBehavior
{
	SW_RTTI( VehicleItem, SWBehavior );

public:
	
	VehicleItem( factory_constructor );
	~VehicleItem();

protected:

	virtual void onAwake();
	virtual void onStart();
	virtual void onCollision( SWCollision2D* );

private:


};


#endif //! VehicleItem_h__
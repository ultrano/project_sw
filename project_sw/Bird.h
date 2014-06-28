#ifndef Bird_h__
#define Bird_h__

#include "SWHeaders.h"

class Bird : public SWBehavior
{
	SW_RTTI( Bird, SWBehavior );

public:



protected:

	void onAwake();
	void onUpdate();

private:


};

#endif
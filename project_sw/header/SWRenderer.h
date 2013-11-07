#ifndef SWRenderer_h__
#define SWRenderer_h__

#include "SWComponent.h"

class SWRenderer : public SWComponent
{
	SW_RTTI( SWRenderer, SWComponent );
	
	void onAdded();
	void onRemoved();

public:

	virtual void render() = 0;

};

#endif // SWRenderer
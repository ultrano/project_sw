#ifndef SWRenderer_h__
#define SWRenderer_h__

#include "SWComponent.h"
#include "SWList.h"

class SWCamera;
class SWRenderer : public SWComponent
{
	SW_RTTI( SWRenderer, SWComponent );

public:

	SWRenderer();
	SWRenderer( factory_constructor );
	~SWRenderer();

	void preRender();
	virtual void render( SWCamera* ) = 0;

protected:

	void onAwake();
	void onRemove();

};

#endif // SWRenderer
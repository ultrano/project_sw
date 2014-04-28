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

	void addPreRenderDelegate( const SWDelegator* del );
	void removePreRenderDelegate( const SWDelegator* del );

	void preRender();
	virtual void render( SWCamera* ) = 0;

protected:

	void onAwake();
	void onRemove();

private:

	SWList::Type m_preRenderDels;
	SWList::Type m_postRenderDels;
};

#endif // SWRenderer
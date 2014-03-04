#ifndef SWRenderer_h__
#define SWRenderer_h__

#include "SWComponent.h"
#include "SWList.h"

class SWRenderer : public SWComponent
{
	SW_RTTI( SWRenderer, SWComponent );
	
private:

	SWList::Type m_preRenderDels;
	SWList::Type m_postRenderDels;

protected:

	void onStart();
	void onRemove();

public:

	void addPreRenderDelegate( const SWDelegator* del );
	void removePreRenderDelegate( const SWDelegator* del );

	void preRender();
	virtual void render() = 0;

};

#endif // SWRenderer
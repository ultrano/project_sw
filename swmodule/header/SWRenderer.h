#ifndef SWRenderer_h__
#define SWRenderer_h__

#include "SWComponent.h"
#include "SWList.h"

class SWRenderer : public SWComponent
{
	SW_RTTI( SWRenderer, SWComponent );
	
private:

	SWList::Value m_preRenderDels;
	SWList::Value m_postRenderDels;

protected:

	void onStart();
	void onRemove();

public:

	void addPreRenderDelegate( const SWDelegate* del );
	void removePreRenderDelegate( const SWDelegate* del );

	void preRender();
	virtual void render() = 0;

};

#endif // SWRenderer
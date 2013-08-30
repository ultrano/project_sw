#ifndef SWMeshRenderer_h__
#define SWMeshRenderer_h__

#include "SWComponent.h"

class SWMeshFilter;

class SWMeshRenderer : public SWComponent
{
	SW_RTTI( SWMeshRenderer, SWComponent );

	unsigned int m_texID;
	SWWeakRef<SWMeshFilter> m_filter;

public:

	void setTexture( unsigned int texID );
	unsigned int getTexture();
	void render();

	void onAdded();
};

#endif // SWMeshRenderer_h__
#ifndef SWMeshRenderer_h__
#define SWMeshRenderer_h__

#include "SWRenderer.h"
#include "SWMatrix4x4.h"
class SWMeshFilter;

class SWMeshRenderer : public SWRenderer
{
	SW_RTTI( SWMeshRenderer, SWRenderer );

	unsigned int m_texID;
	SWMatrix4x4  m_texMat;
	SWWeakRef<SWMeshFilter> m_filter;

public:
	
	void render();

	void setTexture( unsigned int texID );
	unsigned int getTexture();

	void setMeshFilter( SWMeshFilter* filter );
	SWMeshFilter* getMeshFilter();

	void onStart();
};

#endif // SWMeshRenderer_h__
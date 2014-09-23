#ifndef SWMeshRenderer_h__
#define SWMeshRenderer_h__

#include "SWRenderer.h"
#include "TMatrix4x4.h"
class SWMeshFilter;
class SWMaterial;
class SWTexture;

class SWMeshRenderer : public SWRenderer
{
	SW_RTTI( SWMeshRenderer, SWRenderer );

public:
	
	SWMeshRenderer();
	SWMeshRenderer( factory_constructor );
	~SWMeshRenderer();

	void render( SWCamera* );

	void setMeshFilter( SWMeshFilter* filter );
	SWMeshFilter* getMeshFilter();

private:

	void onAwake();

private:

	SWWeakRef<SWMeshFilter> m_filter;
};

#endif // SWMeshRenderer_h__
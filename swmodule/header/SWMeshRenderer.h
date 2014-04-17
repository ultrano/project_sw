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

	SWHardRef<SWTexture> m_texture;
	TMatrix4x4  m_texMat;
	SWWeakRef<SWMeshFilter> m_filter;
	SWHardRef<SWMaterial> m_material;

public:
	
	SWMeshRenderer();
	~SWMeshRenderer();

	void render();

	void setTexture( SWTexture* texture );
	SWTexture* getTexture();

	void setMeshFilter( SWMeshFilter* filter );
	SWMeshFilter* getMeshFilter();

	void setMaterial( SWMaterial* material );
	SWMaterial* getMaterial() const;

	void onAwake();
};

#endif // SWMeshRenderer_h__
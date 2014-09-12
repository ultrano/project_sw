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

	void setMaterial( SWMaterial* material );
	SWMaterial* getMaterial() const;

protected:

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	void onAwake();

private:

	SWWeakRef<SWMeshFilter> m_filter;
	SWHardRef<SWMaterial> m_material;
};

#endif // SWMeshRenderer_h__
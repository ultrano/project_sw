#ifndef SWMeshFilter_h__
#define SWMeshFilter_h__

#include "SWComponent.h"

class SWMesh;

class SWMeshFilter : public SWComponent
{
	SW_RTTI( SWMeshFilter, SWComponent );
public:
	
	SWMeshFilter();
	SWMeshFilter( factory_constructor );
	~SWMeshFilter();
	void setMesh( SWMesh* data );
	SWMesh* getMesh() const;

	void draw();

private:

	void onAwake();
	void onRemove();

private:

	SWHardRef<SWMesh> m_mesh;

};
#endif // SWMeshFilter_h__
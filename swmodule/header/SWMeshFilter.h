#ifndef SWMeshFilter_h__
#define SWMeshFilter_h__

#include "SWComponent.h"

class SWMesh;

class SWMeshFilter : public SWComponent
{
	SW_RTTI( SWMeshFilter, SWComponent );

	SWHardRef<SWMesh> m_mesh;
public:
	
	void setMesh( SWMesh* data );
	SWMesh* getMesh() const;

	void draw();

	void onAwake();
	void onRemove();
};
#endif // SWMeshFilter_h__
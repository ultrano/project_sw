#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWComponent.h"
#include <vector>

class SWMeshData;

class SWMesh : public SWComponent
{
	SW_RTTI( SWMesh, SWComponent );

	SWHardRef<SWMeshData> m_data;
	public:
	
		void setData( SWMeshData* data );
	void render();

	void onAdded();
	void onRemoved();
};
#endif // SWMesh_h__
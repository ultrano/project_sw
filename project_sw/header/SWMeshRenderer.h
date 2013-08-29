#ifndef SWMeshRenderer_h__
#define SWMeshRenderer_h__

#include "SWComponent.h"

class SWMesh;

class SWMeshRenderer : public SWComponent
{
	SW_RTTI( SWMeshRenderer, SWComponent );

	SWWeakRef<SWMesh> m_mesh;

public:

	void render();

	void onAdded();
};

#endif // SWMeshRenderer_h__
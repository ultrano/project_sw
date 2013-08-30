#ifndef SWMeshRenderer_h__
#define SWMeshRenderer_h__

#include "SWComponent.h"

class SWMesh;

class SWMeshRenderer : public SWComponent
{
	SW_RTTI( SWMeshRenderer, SWComponent );

	unsigned int m_texID;
	SWWeakRef<SWMesh> m_mesh;

public:

	void setTexture( unsigned int texID );
	unsigned int getTexture();
	void render();

	void onAdded();
};

#endif // SWMeshRenderer_h__
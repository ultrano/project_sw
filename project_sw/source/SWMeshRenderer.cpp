
#include "SWMeshRenderer.h"
#include "SWGameContext.h"
#include "SWMesh.h"
#include "SWGameObject.h"
#include "SWTransform.h"

void SWMeshRenderer::render()
{
	SWTransform* transform = getGameObject()->getComponent<SWTransform>();
	if ( m_mesh.isValid() )
	{
		SW_GC.setModelMatrix( transform->getWorldMatrix() );
		m_mesh()->render();
	}

	SWTransform::TransformList children;
	transform->copyChildren( children );

	SWTransform::TransformList::iterator itor = children.begin();
	for ( ; itor != children.end() ; ++itor )
	{
		SWGameObject* object = (*itor)()->getGameObject();
		SWMeshRenderer* renderer = object->getComponent<SWMeshRenderer>();
		if ( renderer ) renderer->render();
	}
}

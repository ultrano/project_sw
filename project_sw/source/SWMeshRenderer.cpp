
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

	SWTransform::ChildList children;
	transform->copyChildren( children );

	SWTransform::ChildList::iterator itor = children.begin();
	for ( ; itor != children.end() ; ++itor )
	{
		SWMeshRenderer* renderer = (*itor)()->getComponent<SWMeshRenderer>();
		if ( renderer ) renderer->render();
	}
}

void SWMeshRenderer::onAdded()
{
	m_mesh = getGameObject()->getComponent<SWMesh>();
}

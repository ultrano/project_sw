
#include "SWMeshRenderer.h"
#include "SWGameContext.h"
#include "SWMeshFilter.h"
#include "SWGameObject.h"
#include "SWTransform.h"

void SWMeshRenderer::render()
{
	SWTransform* transform = getGameObject()->getComponent<SWTransform>();
	if ( m_filter.isValid() )
	{
		SW_GC.setModelMatrix( transform->getWorldMatrix() );
		SW_GC.bindTexture( m_texID );
		m_filter()->render();
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
	m_filter = getGameObject()->getComponent<SWMeshFilter>();
}

void SWMeshRenderer::setTexture( unsigned int texID )
{
	m_texID = texID;
}

unsigned int SWMeshRenderer::getTexture()
{
	return m_texID;
}

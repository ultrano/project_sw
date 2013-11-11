
#include "SWMeshRenderer.h"
#include "SWGameContext.h"
#include "SWMeshFilter.h"
#include "SWGameObject.h"
#include "SWTransform.h"

void SWMeshRenderer::render()
{
	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	if ( m_filter.isValid() )
	{
		SW_GC.setModelMatrix( transform->getFinalMatrix() );
		SW_GC.bindTexture( m_texID );
		m_filter()->draw();
	}

	SWObjectList children;
	transform->copyChildren( children );

	SWObjectList::iterator itor = children.begin();
	for ( ; itor != children.end() ; ++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		SWMeshRenderer* renderer = go->getComponent<SWMeshRenderer>();
		if ( renderer ) renderer->render();
	}
}

void SWMeshRenderer::onStart()
{
	__super::onStart();
	m_filter = gameObject()->getComponent<SWMeshFilter>();
}

void SWMeshRenderer::setTexture( unsigned int texID )
{
	m_texID = texID;
}

unsigned int SWMeshRenderer::getTexture()
{
	return m_texID;
}

void SWMeshRenderer::setMeshFilter( SWMeshFilter* filter )
{
	m_filter = filter;
}

SWMeshFilter* SWMeshRenderer::getMeshFilter()
{
	return m_filter();
}
#include "SWMeshRenderer.h"
#include "SWGameContext.h"
#include "SWMeshFilter.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMaterial.h"
#include "SWShader.h"
#include "SWCamera.h"

SWMeshRenderer::SWMeshRenderer()
{
}
SWMeshRenderer::~SWMeshRenderer()
{
}

void SWMeshRenderer::render()
{
	if ( m_material.isValid() )
	{
		SWTransform* transform = gameObject()->getComponent<SWTransform>();
		const TMatrix4x4& model = transform->getWorldMatrix();
		const TMatrix4x4& view = SWCamera::mainCamera()->getViewMatrix();
		const TMatrix4x4& proj = SWCamera::mainCamera()->getProjMatrix();
		m_material()->setMatrix4x4( "u_mvpMat", ( model * view * proj ) );
		m_material()->setTexture( "s_texture", m_texID );
		m_material()->apply();
	}
	if ( m_filter.isValid() )
	{
		m_filter()->draw();
	}
}

void SWMeshRenderer::onStart()
{
	__super::onStart();
	m_filter = gameObject()->getComponent<SWMeshFilter>();
	m_material = SW_GC.getDefaultMaterial();
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

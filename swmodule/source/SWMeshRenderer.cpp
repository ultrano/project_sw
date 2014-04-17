#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMaterial.h"
#include "SWShader.h"
#include "SWCamera.h"
#include "SWTexture.h"

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
		m_material()->setMatrix4x4( "MATRIX_MVP", ( model * view * proj ) );
		m_material()->setTexture( "TEXTURE_0", m_texture() );
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
	m_material = new SWMaterial();
	m_material()->setVector4( "COLOR", tquat(1,1,1,0.5) );
}

void SWMeshRenderer::setTexture( SWTexture* texture )
{
	m_texture = texture;
}

SWTexture* SWMeshRenderer::getTexture()
{
	return m_texture();
}

void SWMeshRenderer::setMeshFilter( SWMeshFilter* filter )
{
	m_filter = filter;
}

SWMeshFilter* SWMeshRenderer::getMeshFilter()
{
	return m_filter();
}

void SWMeshRenderer::setMaterial( SWMaterial* material )
{
	m_material = material;
}

SWMaterial* SWMeshRenderer::getMaterial() const
{
	return m_material();
}

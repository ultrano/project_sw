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
		const TMatrix4x4& vpMat = SWCamera::mainCamera()->getVPMatrix();
		m_material()->setMatrix4x4( "MATRIX_MVP", ( model * vpMat ) );
		m_material()->apply();
	}
	if ( m_filter.isValid() )
	{
		m_filter()->draw();
	}
}

void SWMeshRenderer::onAwake()
{
	__super::onAwake();
	m_filter = gameObject()->getComponent<SWMeshFilter>();
	m_material = new SWMaterial();
	m_material()->setVector4( "COLOR", tquat(1,1,1,1) );
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

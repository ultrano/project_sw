#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMaterial.h"
#include "SWShader.h"
#include "SWCamera.h"
#include "SWTexture.h"
#include "SWObjectStream.h"
#include "SWMesh.h"

SWMeshRenderer::SWMeshRenderer()
{
	getMaterial()->setVector4( "COLOR", tquat(1,1,1,1) );
}

SWMeshRenderer::SWMeshRenderer( factory_constructor )
{
	getMaterial()->setVector4( "COLOR", tquat(1,1,1,1) );
}

SWMeshRenderer::~SWMeshRenderer()
{
}

void SWMeshRenderer::render( SWCamera* camera )
{
	if ( getMaterial() )
	{
		SWTransform* transform = gameObject()->getComponent<SWTransform>();
		const TMatrix4x4& model = transform->getWorldMatrix();
		const TMatrix4x4& vpMat = camera->getVPMatrix();
		getMaterial()->setMatrix4x4( "MATRIX_MVP", ( model * vpMat ) );
		getMaterial()->apply();
	}
	
	if ( getMesh() != m_filter()->getMesh() ) setMesh( m_filter()->getMesh() );
	if ( getMesh() ) getMesh()->draw();
}

void SWMeshRenderer::onAwake()
{
	__super::onAwake();
	m_filter = gameObject()->getComponent<SWMeshFilter>();
	setMesh( m_filter()->getMesh() );
}

void SWMeshRenderer::setMeshFilter( SWMeshFilter* filter )
{
	m_filter = filter;
}

SWMeshFilter* SWMeshRenderer::getMeshFilter()
{
	return m_filter();
}
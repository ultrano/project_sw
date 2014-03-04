#include "SWCamera.h"
#include "SWGameObject.h"
#include "SWTransform.h"

SWHardRef<SWCamera> SWCamera::mainCamera = NULL;

void SWCamera::onAwake()
{
	__super::onAwake();
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWCamera::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
	__super::onRemove();
}

void SWCamera::onUpdate()
{
}

void SWCamera::orthoMode( float width, float height, float near, float far )
{
	float halfW = width/2;
	float halfH = height/2;
	m_projMatrix.ortho( -halfW, halfW, -halfH, halfH, near, far );
}

void SWCamera::perspectiveMode( float fov, float aspect, float near, float far )
{
	m_projMatrix.perspective( fov, aspect, near, far );
}

const TMatrix4x4& SWCamera::getProjMatrix() const
{
	return m_projMatrix;
}

const TMatrix4x4& SWCamera::getViewMatrix()
{
	SWTransform* trans = getComponent<SWTransform>();
	trans->getWorldMatrix().inverse( m_viewMatrix );
	return m_viewMatrix;
}
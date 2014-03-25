#include "SWCamera.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWGameContext.h"

SWHardRef<SWCamera> SWCamera::mainCamera = NULL;

SWCamera::SWCamera()
	: m_near( 0 )
	, m_far( 0 )
{
}

SWCamera::~SWCamera()
{
}

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
	m_near = near;
	m_far  = far;
	m_projMatrix.ortho( -halfW, halfW, -halfH, halfH, near, far );
	m_projMatrix.inverse( m_invProjMatrix );
}

void SWCamera::perspectiveMode( float fov, float aspect, float near, float far )
{
	m_near = near;
	m_far  = far;
	m_projMatrix.perspective( fov, aspect, near, far );
	m_projMatrix.inverse( m_invProjMatrix );
}

tvec3 SWCamera::screenToWorld( const tvec3& screenPt ) const
{
	tvec3 ret( screenPt );
	ret.x = ret.x / SW_GC.getScreenWidth();
	ret.y = ret.y / SW_GC.getScreenHeight();
	ret.z = (ret.z - m_near) / ( m_far - m_near );

	ret.x = (+ret.x*2.0f) - 1.0f;
	ret.y = (-ret.y*2.0f) + 1.0f;
	ret.z = (+ret.z*2.0f) - 1.0f;
	
	SWTransform* trans = getComponent<SWTransform>();
	ret = ret * m_invProjMatrix * trans->getWorldMatrix();
	return ret;
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
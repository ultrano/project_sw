#include "SWCamera.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWDefines.h"
#include "SWObjectStream.h"
#include "SWMath.h"

SWCamera::SWCamera()
	: m_near( 0 )
	, m_far( 0 )
	, m_clearColor( 0, 0, 1, 1 )
	, m_clearDepth( 0 )
	, m_depth( 0 )
	, m_clearFlags( SW_Dont_Clear )
{
}

SWCamera::SWCamera( factory_constructor )
	: m_near( 0 )
	, m_far( 0 )
	, m_clearColor( 0, 0, 1, 1 )
	, m_clearDepth( 0 )
	, m_depth( 0 )
	, m_clearFlags( SW_Dont_Clear )
{

}

SWCamera::~SWCamera()
{
}

void SWCamera::onAwake()
{
	__super::onAwake();
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );

	m_cullingMask.clear(false);
	m_cullingMask.set(SW_DefaultLayer,true);
	SW_GC.getScene()->addCamera( this );
}

void SWCamera::onRemove()
{
	SW_GC.getScene()->removeCamera( this );
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
	__super::onRemove();
}

void SWCamera::onUpdate()
{
	SWTransform* trans = getComponent<SWTransform>();
	const tmat44& worldMat = trans->getWorldMatrix();
	worldMat.inverse( m_viewMatrix );
	m_vpMatrix = m_viewMatrix * m_projMatrix;

	{
		tquat quat(0,0,1,0);
		quat = (quat * worldMat);
		quat = quat.normal();
		m_lookDir = quat.vec();
	}
	{
		tquat quat(0,1, 0,0);
		quat = (quat * worldMat);
		quat = quat.normal();
		m_upDir = quat.vec();
	}
	{
		tquat quat(1,0, 0,0);
		quat = (quat * worldMat);
		quat = quat.normal();
		m_rightDir = quat.vec();
	}
}

void SWCamera::orthoMode( float width, float height, float near, float far )
{
	float halfW = width/2;
	float halfH = height/2;
	m_near = near;
	m_far  = far;
	m_projMatrix.ortho( -halfW, halfW, -halfH, halfH, near, far );
	m_projMatrix.inverse( m_invProjMatrix );

	m_frustrumAABB.set( tvec3( -halfW, -halfH, near ), tvec3( halfW, halfH, far ) );
}

void SWCamera::perspectiveMode( float fov, float aspect, float near, float far )
{
	m_near = near;
	m_far  = far;
	m_projMatrix.perspective( fov, aspect, near, far );
	m_projMatrix.inverse( m_invProjMatrix );

	float halfW = SWMath.tan( fov ) * far;
	float halfH = halfW * aspect;

	m_frustrumAABB.set( tvec3( -halfW, -halfH, near ), tvec3( halfW, halfH, near ) );
}

bool SWCamera::computeFrustrumAABB( taabb3d& aabb ) const
{
	SWTransform* trans = getComponent<SWTransform>();
	if ( trans == NULL ) return false;

	const tmat44 worldMat = trans->getWorldMatrix();

	tvec3 point1 = m_frustrumAABB.lower * worldMat;
	tvec3 point2 = m_frustrumAABB.upper * worldMat;

	aabb.set( point1, point2 );
	return true;
}

tvec3 SWCamera::screenToWorld( const tvec3& screenPt ) const
{
	tvec3 ret( 0, 0, screenPt.z );
	ret = ret* m_projMatrix;
	ret.x = (+(screenPt.x/ SW_GC.getScreenWidth())* 2.0f)- 1.0f;
	ret.y = (-(screenPt.y/ SW_GC.getScreenHeight())* 2.0f)+ 1.0f;

	SWTransform* trans = getComponent<SWTransform>();
	ret = ret* m_invProjMatrix;
	ret = ret* trans->getWorldMatrix();
	return ret;
}

tray SWCamera::screenToRay( const tvec2& screenPt ) const
{
	tvec3 ret( 0, 0, m_near );
	ret = ret* m_projMatrix;
	ret.x = (+(screenPt.x/ SW_GC.getScreenWidth())* 2.0f)- 1.0f;
	ret.y = (-(screenPt.y/ SW_GC.getScreenHeight())* 2.0f)+ 1.0f;

	SWTransform* trans = getComponent<SWTransform>();
	ret = ret* m_invProjMatrix;
	ret = ret* trans->getWorldMatrix();

	tray ray;
	ray.origin = ret;
	ray.direction = (ret- trans->getPosition()).normal();
	return ray;
}

const TMatrix4x4& SWCamera::getProjMatrix() const
{
	return m_projMatrix;
}

const TMatrix4x4& SWCamera::getViewMatrix() const
{
	return m_viewMatrix;
}

const TMatrix4x4& SWCamera::getVPMatrix() const
{
	return m_vpMatrix;
}

void SWCamera::setClearColor( const tcolor& color )
{
	m_clearColor = color;
}

const tcolor& SWCamera::getClearColor() const
{
	return m_clearColor;
}

void SWCamera::setClearDepth( float depth )
{
	m_clearDepth = depth;
}

float SWCamera::getClearDepth() const
{
	return m_clearDepth;
}

tflag32 SWCamera::getCullingMask() const
{
	return m_cullingMask;
}

void SWCamera::setCullingMask( tflag32 mask )
{
	if ( mask.flags == 0 ) return;
	if ( m_cullingMask == mask ) return;

	m_cullingMask = mask;
}

void SWCamera::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );
	writer->writeFloat( m_clearDepth );
	writer->writeFloat( m_near );
	writer->writeFloat( m_far );
	writer->writeInt( m_depth );
	writer->writeInt( m_clearFlags );
	writer->writeUInt( m_cullingMask.flags );
	writer->writeColor( m_clearColor );
}

void SWCamera::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );
	m_clearDepth = reader->readFloat();
	m_near  = reader->readFloat();
	m_far   = reader->readFloat();
	m_depth = reader->readInt();
	m_clearFlags  = reader->readInt();
	m_cullingMask.flags = reader->readUInt();
	reader->readColor( m_clearColor );

}
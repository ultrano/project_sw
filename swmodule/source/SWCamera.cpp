#include "SWCamera.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWDefines.h"
#include "SWObjectStream.h"

SWCamera::SWCamera()
	: m_near( 0 )
	, m_far( 0 )
	, m_clearColor( 0, 0, 1, 1 )
	, m_clearDepth( 0 )
	, m_layerName( "default" )
	, m_depth( 0 )
	, m_clearFlags( SW_Dont_Clear )
{
}

SWCamera::SWCamera( factory_constructor )
	: m_near( 0 )
	, m_far( 0 )
	, m_clearColor( 0, 0, 1, 1 )
	, m_clearDepth( 0 )
	, m_layerName( "default" )
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
	SW_GC.getScene()->m_cameras.push_back( this );
}

void SWCamera::onRemove()
{
	SW_GC.getScene()->m_cameras.remove( this );
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

const thashstr& SWCamera::getTargetLayerName() const
{
	return m_layerName;
}

void SWCamera::setTargetLayerName( const thashstr& name )
{
	m_layerName = name;
}

void SWCamera::serialize( SWObjectWriter* ow )
{
	__super::serialize( ow );
	ow->writeFloat( m_clearDepth );
	ow->writeFloat( m_near );
	ow->writeFloat( m_far );
	ow->writeInt( m_depth );
	ow->writeInt( m_clearFlags );
	ow->writeColor( m_clearColor );
	ow->writeString( m_layerName.str() );
}

void SWCamera::deserialize( SWObjectReader* or )
{
	__super::deserialize( or );
	m_clearDepth = or->readFloat();
	m_near  = or->readFloat();
	m_far   = or->readFloat();
	m_depth = or->readInt();
	m_clearFlags = or->readInt();
	or->readColor( m_clearColor );

	tstring layerName;
	or->readString( layerName );
	m_layerName = layerName;
}
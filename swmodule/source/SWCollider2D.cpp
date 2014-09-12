#include "SWCollider2D.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWPhysics2D.h"
#include "SWObjectStream.h"
#include "SWRigidBody2D.h"

void SWCollider2D::onAwake()
{
	__super::onAwake();
	SWPhysics2D.m_colliders.push_back( this );
}

void SWCollider2D::onRemove()
{
	SWPhysics2D.m_colliders.remove( this );
	__super::onRemove();
}

////////////////////////////////////////////////////////////////

SWCircleCollider2D::SWCircleCollider2D( factory_constructor )
	: m_center( 0, 0 )
	, m_radius( 0 )
{
}

SWCircleCollider2D::~SWCircleCollider2D()
{
}

float SWCircleCollider2D::getWorldRadius() const
{
	tquat radius( m_radius, m_radius, m_radius, 0 );
	radius = radius * getComponent<SWTransform>()->getWorldMatrix();
	return SWMath.max( radius.x, SWMath.max( radius.y, radius.z ) );
}

tvec2 SWCircleCollider2D::getWorldCenter() const
{
	tvec3 pos( m_center.x, m_center.y, 0 );
	pos = pos * getComponent<SWTransform>()->getWorldMatrix();
	return pos.xy();
}

bool SWCircleCollider2D::containPoint( const tvec2& point )
{
	SWTransform* transform = getComponent<SWTransform>();
	tvec3 center = tvec3(m_center, 0) * transform->getWorldMatrix();
	return ( (center.xy() - point).length() <= m_radius );
}

void SWCircleCollider2D::farthestPoint( tvec2& ret, const tvec2& worldDir )
{
	tvec2 center = getWorldCenter();
	float radius = getWorldRadius();

	SWRigidBody2D* body = getComponent<SWRigidBody2D>();
	if ( body )
	{
		tvec2 velocity = body->getVelocity();
		if ( worldDir.dot( velocity ) > 0 ) center += velocity;
	}

	ret = center + (worldDir * radius);
}

void SWCircleCollider2D::serialize( SWObjectWriter* writer )
{
	writer->writeVec2( m_center );
	writer->writeFloat( m_radius );
}

void SWCircleCollider2D::deserialize( SWObjectReader* reader )
{
	reader->readVec2( m_center );
	m_radius = reader->readFloat();
}

////////////////////////////////////////////////////////////////

SWRectCollider2D::SWRectCollider2D( factory_constructor )
	: m_center( 0, 0 )
	, m_size( 0, 0 )
{
}

SWRectCollider2D::~SWRectCollider2D()
{
}

tvec2 SWRectCollider2D::getWorldCenter() const
{
	tvec3 pos( m_center.x, m_center.y, 0 );
	pos = pos * getComponent<SWTransform>()->getWorldMatrix();
	return pos.xy();
}

void SWRectCollider2D::getWorldEdges( tvec2& edge1, tvec2& edge2, tvec2& edge3, tvec2& edge4 ) const
{
	float halfW = m_size.x/2;
	float halfH = m_size.y/2;
	const tmat44& worldMat = getComponent<SWTransform>()->getWorldMatrix();

	edge1 = (tvec3( -halfW, +halfH, 0 ) * worldMat).xy();
	edge2 = (tvec3( -halfW, -halfH, 0 ) * worldMat).xy();
	edge3 = (tvec3( +halfW, +halfH, 0 ) * worldMat).xy();
	edge4 = (tvec3( +halfW, -halfH, 0 ) * worldMat).xy();
}

void SWRectCollider2D::getSREdges( tvec2& edge1, tvec2& edge2, tvec2& edge3, tvec2& edge4 ) const
{
	float halfW = m_size.x/2;
	float halfH = m_size.y/2;
	const tmat44& worldMat = getComponent<SWTransform>()->getWorldMatrix();

	edge1 = (tquat( -halfW, +halfH, 0, 0 ) * worldMat).xy();
	edge2 = (tquat( -halfW, -halfH, 0, 0 ) * worldMat).xy();
	edge3 = (tquat( +halfW, +halfH, 0, 0 ) * worldMat).xy();
	edge4 = (tquat( +halfW, -halfH, 0, 0 ) * worldMat).xy();
}

bool SWRectCollider2D::containPoint( const tvec2& point )
{
	SWTransform* transform = getComponent<SWTransform>();
	tvec3 point3 = tvec3( point, 0 ) * transform->getInvWorldMatrix();
	tvec2 center = point3.xy() - m_center;
	return SWMath.abs( center.x ) < m_size.x/2.0f && 
	       SWMath.abs( center.y ) < m_size.y/2.0f;
}

void SWRectCollider2D::farthestPoint( tvec2& ret, const tvec2& worldDir )
{
	tvec2 origin1, origin2, origin3, origin4;
	tvec2 edge1, edge2, edge3, edge4;
	getWorldEdges( edge1, edge2, edge3, edge4 );
	ret = edge1;
	float len = worldDir.dot( edge1 );
	float temp = len;
	if (( temp = worldDir.dot( edge2 ) ) > len)
	{
		len = temp;
		ret = edge2;
	}
	if (( temp = worldDir.dot( edge3 ) ) > len)
	{
		len = temp;
		ret = edge3;
	}
	if (( temp = worldDir.dot( edge4 ) ) > len)
	{
		len = temp;
		ret = edge4;
	}
}

void SWRectCollider2D::serialize( SWObjectWriter* writer )
{
	writer->writeVec2( m_center );
	writer->writeVec2( m_size );
}

void SWRectCollider2D::deserialize( SWObjectReader* reader )
{
	reader->readVec2( m_center );
	reader->readVec2( m_size );
}
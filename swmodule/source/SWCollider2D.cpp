#include "SWCollider2D.h"
#include "SWTransform.h"
#include "SWMath.h"
#include "SWPhysics2D.h"

void SWCollider2D::onAwake()
{
	__super::onAwake();
	SWPhysics.m_colliders.push_back( this );
}

void SWCollider2D::onRemove()
{
	SWPhysics.m_colliders.remove( this );
	__super::onRemove();
}

////////////////////////////////////////////////////////////////

SWCircleCollider2D::SWCircleCollider2D( factory_constructor )
{
}

SWCircleCollider2D::~SWCircleCollider2D()
{
}

bool SWCircleCollider2D::containPoint( const tvec2 point )
{
	SWTransform* transform = getComponent<SWTransform>();
	tvec3 center = tvec3(m_center, 0)* transform->getWorldMatrix();
	return ( (center.xy()- point).length() <= m_radius );
}

////////////////////////////////////////////////////////////////

SWRectCollider2D::SWRectCollider2D( factory_constructor )
{
}

SWRectCollider2D::~SWRectCollider2D()
{
}

bool SWRectCollider2D::containPoint( const tvec2 point )
{
	SWTransform* transform = getComponent<SWTransform>();
	tvec3 point3 = tvec3( point, 0 ) * transform->getInvWorldMatrix();
	tvec2 center = point3.xy() - m_center;
	return SWMath.abs( center.x ) < m_size.x/2.0f && 
	       SWMath.abs( center.y ) < m_size.y/2.0f;
}
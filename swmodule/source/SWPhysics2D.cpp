#include "SWPhysics2D.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWMath.h"
#include "SWLog.h"

__SWPhysics2D& __SWPhysics2D::instance()
{
	static __SWPhysics2D s_instance;

	return s_instance;
}


__SWPhysics2D::__SWPhysics2D()
	: m_gravityForce( 0.985f )
{
}

__SWPhysics2D::~__SWPhysics2D()
{
}

void __SWPhysics2D::simulate()
{
}

bool __SWPhysics2D::getIgnoreLayer( tuint layer1, tuint layer2 )
{
	return false;
}

void __SWPhysics2D::ignoreLayer( const thashstr& layer1, const thashstr& layer2, bool ignore )
{
}

SWCollider2D* __SWPhysics2D::overlapPoint( const tvec2& point )
{
	return NULL;
}

float __SWPhysics2D::getFixedInterval()
{
	static const float interval = 1.0f/30.0f;
	return interval;
}

bool __SWPhysics2D::testCollide( SWCollider2D* collider1, SWCollider2D* collider2 )
{
	return false;
}
